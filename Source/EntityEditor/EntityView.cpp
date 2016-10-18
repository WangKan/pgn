#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Graphics/Camera.h>
#include <PGN/Graphics/DirectionalLight.h>
#include <PGN/Graphics/Entity.h>
#include <PGN/Graphics/Graphics.h>
#include <PGN/Graphics/Model.h>
#include <PGN/Graphics/Scene.h>
#include <PGN/Graphics/SceneDirectionalLight.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Math/Math.h>
#include <PGN/Platform/DebugHeap.h>
#include <PGN/Platform/UI/Gesture/DraggingGestureRecognizer.h>
#include <PGN/Platform/UI/Window.h>
#include <PGN/Utilities/Clock.h>
#include <PGN/Utilities/LinearTransformations.h>
#include <PGN/Utilities/SkeletalAnimation/Animation.h>
#include <PGN/Utilities/SkeletalAnimation/AnimationFactory.h>
#include <PGN/Utilities/SkeletalAnimation/Skeleton.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonFactory.h>
#include "EntityView.h"

EntityView::EntityView(IntPtr^ hWnd, Options^ options)
{
	this->options = options;

	_isDisposed = false;

	_beginDebugHeap();

	wnd = pgn::Window::create(8, 8, 8, 8, 24, 8, 1, (HWND)hWnd->ToPointer());

	std::string tempDir;

	char* str = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(options->tempDir);
	tempDir = str;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)str));

	assetStream1 = pgn::createStdFileStream(tempDir.c_str());
	assetStream2 = pgn::createStdFileStream(tempDir.c_str());
	cacheStream = pgn::createStdFileStream("");

	graphics = pgn::Graphics::create(wnd->getDisplay(), assetStream1, /*cacheStream*/0);
	graphics->beginDraw(wnd);

	scene = graphics->createScene();

	camera = graphics->createCamera();

	dirLight = graphics->createDirectionalLight();
	sceneDirLight = scene->add(dirLight);

	entity = graphics->createEntity();

	model = graphics->createModel();
	entity->setModel(model);

	skelFactory = pgn::SkeletonFactory::create();
	skel = skelFactory->createSkeleton();
	entity->setSkeleton(skel);

	sceneEntity = scene->add(entity, true);
	sceneEntity->setScale(1.0f, 1.0f);

	animFactory = pgn::AnimationFactory::create(assetStream2);
	anims = new std::map<std::string, pgn::Animation*>;

	clock = pgn::Clock::create(1, 4800);
	t = (int)clock->getTickCount();

	draggingGestureRecognizer = pgn::DraggingGestureRecognizer::create();
	wnd->addGestureRecognizer(draggingGestureRecognizer);

	viewMatDirty = true;
}

EntityView::~EntityView()
{
	scene->remove(sceneDirLight);
	scene->remove(sceneEntity);
	scene->destroy();

	camera->destroy();
	dirLight->destroy();

	entity->destroy();
	model->destroy();
	skel->destroy();
	skelFactory->destroy();
	for (auto& entry : *anims) entry.second->destroy();
	delete anims;
	animFactory->destroy();
	destroyStdFileStream(assetStream2);

	graphics->endDraw();
	graphics->destroy();
	destroyStdFileStream(assetStream1);
	destroyStdFileStream(cacheStream);
	wnd->destroy();

	clock->destroy();

	draggingGestureRecognizer->destroy();

	_endDebugHeap();

	_isDisposed = true;
}

pgn::Float4x3 worldMat =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
};

struct{
	pgn::Float3 min;
	pgn::Float3 max;
}aabb;

pgn::Float3 center;

pgn::Float4x3 viewMat =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
};

pgn::Float4x3 tempViewMat;

pgn::Float3 dir1 =
{
	0, 0, 1
};

pgn::Float3 operator+(const pgn::Float3& a, const pgn::Float3& b)
{
	pgn::Float3 r;
	r[0] = a.v[0] + b.v[0];
	r[1] = a.v[1] + b.v[1];
	r[2] = a.v[2] + b.v[2];

	return r;
}

pgn::Float3 operator-(const pgn::Float3& a, const pgn::Float3& b)
{
	pgn::Float3 r;
	r[0] = a.v[0] - b.v[0];
	r[1] = a.v[1] - b.v[1];
	r[2] = a.v[2] - b.v[2];

	return r;
}

pgn::Float3 operator/(pgn::Float3& v, float t)
{
	pgn::Float3 r;
	r[0] = v[0] / t;
	r[1] = v[1] / t;
	r[2] = v[2] / t;

	return r;
}

float dot(pgn::Float3& a, pgn::Float3& b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float len(pgn::Float3& v)
{
	return sqrt(dot(v, v));
}

#pragma managed(push, off)
void initViewMat(float minFov)
{
	center = (aabb.min + aabb.max) / 2.0f;

	float r = len(center - aabb.min);
	float dist = r / sin(minFov / 2.0f);

	viewMat[0][0] = 1.0f;		viewMat[1][0] = 0.0f;		viewMat[2][0] = 0.0f;
	viewMat[0][1] = 0.0f;		viewMat[1][1] = 1.0f;		viewMat[2][1] = 0.0f;
	viewMat[0][2] = 0.0f;		viewMat[1][2] = 0.0f;		viewMat[2][2] = 1.0f;
	viewMat[0][3] = -center[0];	viewMat[1][3] = -center[1];	viewMat[2][3] = -center[2] + dist;
}
#pragma managed(pop)

#pragma managed(push, off)
void copy(pgn::Float4x3& dest, pgn::Float4x3& src)
{
	dest = src;
}
#pragma managed(pop)

void EntityView::update(EntityProperties^ props, DirtyFlags^ dirtyFlags)
{
	using namespace System::Runtime::InteropServices;

	float aspectRatio = (float)wnd->getClientWidth() / (float)wnd->getClientHeight();
	bool vFov = options->verticalFovFixed ? options->fov : options->fov / aspectRatio;

	camera->setFrustumFovLH(vFov, aspectRatio, 0.01f, 1024);
	camera->setViewport(0, 0, wnd->getClientWidth(), wnd->getClientHeight(), wnd->getClientHeight());

	dirLight->setIntensity(1.0f, 1.0f, 1.0f, 1.0f);
	sceneDirLight->setDirection(&dir1);

	if (dirtyFlags->mesh)
	{
		char* str;

		str = (char*)(void*)Marshal::StringToHGlobalAnsi(props->mesh);
		model->setEditableMesh(str);
		Marshal::FreeHGlobal(System::IntPtr((void*)str));

		if (props->diffuseMap == "")
		{
			model->setDiffuseMap(0, "gray");
		}
		else
		{
			str = (char*)(void*)Marshal::StringToHGlobalAnsi(options->tempDir + Utilities::getFileName(props->diffuseMap, ".TGA") + ".dxt.PNT");
			model->setDiffuseMap(0, str);
			Marshal::FreeHGlobal(System::IntPtr((void*)str));
		}

		viewMatDirty = true;
	}

	if (viewMatDirty && model->complete())
	{
		model->getAabb(0, &aabb.min, &aabb.max);
		float hFov = vFov * aspectRatio;
		initViewMat(min(vFov, hFov));

		viewMatDirty = false;
	}

	if (dirtyFlags->diffuseMap)
	{
		if (props->diffuseMap == "")
		{
			model->setDiffuseMap(0, "gray");
		}
		else
		{
			char* str = (char*)(void*)Marshal::StringToHGlobalAnsi(options->tempDir + Utilities::getFileName(props->diffuseMap, ".TGA") + ".dxt.PNT");
			model->setDiffuseMap(0, str);
			Marshal::FreeHGlobal(System::IntPtr((void*)str));
		}
	}

	if (dirtyFlags->anims)
	{
		skel->stopAll();

		std::map<std::string, pgn::Animation*> tmp;

		for each(Anim^ animProp in props->anims)
		{
			pgn::Animation* anim = animFactory->createAnimation();

			char* str = (char*)(void*)Marshal::StringToHGlobalAnsi(animProp->fileName);
			anim->set(str);
			tmp[str] = anim;
			Marshal::FreeHGlobal(System::IntPtr((void*)str));
		}

		for (auto& entry : *anims)
			entry.second->destroy();

		*anims = tmp;
	}

	if (dirtyFlags->curAnim)
	{
		for each (Object^ o in options->curAnims)
		{
			Anim^ animProp = (Anim^)o;

			char* str = (char*)(void*)Marshal::StringToHGlobalAnsi(animProp->fileName);
			skel->playAnimation((*anims)[str]);
			Marshal::FreeHGlobal(System::IntPtr((void*)str));
		}
	}

	int _t = (int)clock->getTickCount();
	int dt = _t - t;
	t = _t;

	int animDt = dt;
	skel->updatePose(animDt);

	sceneEntity->setWorldMat(&worldMat);

	draggingGestureRecognizer->update();

	if (draggingGestureRecognizer->getState() == pgn::gestureRecognized)
	{
		int startX, startY;
		draggingGestureRecognizer->getStart(&startX, &startY);

		int x, y;
		draggingGestureRecognizer->getPosition(&x, &y);

		float yaw = -(x - startX) * 2.0f * System::Math::PI / 500.0f;
		float pitch = -(y - startY) * 2.0f * System::Math::PI / 500.0f;

		pgn::buildViewMat(&viewMat, &center, yaw, pitch, &tempViewMat);
		camera->setViewMat(&tempViewMat);
	}
	else if (draggingGestureRecognizer->getState() == pgn::gestureEnded)
	{
		copy(viewMat, tempViewMat);
	}
	else
	{
		camera->setViewMat(&viewMat);
	}

	scene->commit(camera);
}
