#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Graphics/Camera.h>
#include <PGN/Graphics/DirectionalLight.h>
#include <PGN/Graphics/EditableModel.h>
#include <PGN/Graphics/Graphics.h>
#include <PGN/Graphics/NavModel.h>
#include <PGN/Graphics/Scene.h>
#include <PGN/Graphics/SceneDirectionalLight.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Graphics/SkeletalModel.h>
#include <PGN/Math/Math.h>
#include <PGN/Platform/DebugHeap.h>
#include <PGN/Platform/FileStream.h>
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

	wnd = pgn::Window::create(8, 8, 8, 8, 0, 0, 1, (HWND)hWnd->ToPointer());

	assetStream1 = pgn::createStdFileStream("");
	assetStream2 = pgn::createStdFileStream("");
	cacheStream = pgn::createStdFileStream("");

	graphics = pgn::Graphics::create(wnd->getDisplay(), assetStream1, /*cacheStream*/0);
	graphics->beginDraw(wnd);

	scene = graphics->createScene();

	camera = graphics->createCamera();

	skyBox = graphics->createModel();
	skyBox->setMesh("skybox.PNM");
	skyBox->setDiffuseMap(0, "black");
	sceneSkyBox = scene->addSkyBox(skyBox, false);
	sceneSkyBox->setScale(1.0f, 1.0f);

	dirLight = graphics->createDirectionalLight();
	sceneDirLight = scene->add(dirLight);

	skeletalModel = graphics->createSkeletalModel();

	model = graphics->createEditableModel();
	skeletalModel->setModel(model);

	skelFactory = pgn::SkeletonFactory::create();
	skel = skelFactory->createSkeleton();
	skeletalModel->setSkeleton(skel);

	sceneEntity = scene->addSkeletalModel(skeletalModel, true);
	sceneEntity->setScale(1.0f, 1.0f);

	animFactory = pgn::AnimationFactory::create(assetStream2);
	anims = new std::map<std::string, pgn::Animation*>;

	navModel = graphics->createNavModel();
	navModelSceneEntity = scene->addNavModel(navModel, true);
	navModelSceneEntity->setScale(1.0f, 1.0f);

	clock = pgn::Clock::create(1, 4800);
	t = (int)clock->getTickCount();

	draggingGestureRecognizer = pgn::DraggingGestureRecognizer::create();
	wnd->addGestureRecognizer(draggingGestureRecognizer);

	viewMatDirty = false;
}

EntityView::~EntityView()
{
	scene->removeSkyBox(sceneSkyBox);
	scene->remove(sceneDirLight);
	scene->removeSkeletalModel(sceneEntity);
	scene->removeNavModel(navModelSceneEntity);
	scene->destroy();

	skyBox->destroy();
	camera->destroy();
	dirLight->destroy();

	skeletalModel->destroy();
	model->destroy();
	skel->destroy();
	skelFactory->destroy();
	for (auto& entry : *anims) entry.second->destroy();
	delete anims;
	animFactory->destroy();
	navModel->destroy();
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

class Aabb
{
public:
	pgn::Float3 min;
	pgn::Float3 max;

	void init()
	{
		Aabb empty = { { FLT_MAX, FLT_MAX, FLT_MAX }, { -FLT_MAX, -FLT_MAX, -FLT_MAX } };
		min = empty.min;
		max = empty.max;
	}

	void add(pgn::Float3* v)
	{
		if (v->x < min.x) min.x = v->x;
		if (v->y < min.y) min.y = v->y;
		if (v->z < min.z) min.z = v->z;

		if (v->x > max.x) max.x = v->x;
		if (v->y > max.y) max.y = v->y;
		if (v->z > max.z) max.z = v->z;
	}
};

Aabb aabb;

pgn::Float3 center;

pgn::Float4x3 viewMat =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
};

pgn::Float4x3 tempViewMat;

pgn::Float4x3 invViewMat;

pgn::Float4x3 skyBoxWorldMat =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
};

pgn::Float3 dir1 =
{
	0, 0, 1
};

pgn::Float3 operator+(const pgn::Float3& a, const pgn::Float3& b)
{
	pgn::Float3 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;

	return r;
}

pgn::Float3 operator-(const pgn::Float3& a, const pgn::Float3& b)
{
	pgn::Float3 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;

	return r;
}

pgn::Float3 operator/(pgn::Float3& v, float t)
{
	pgn::Float3 r;
	r.x = v.x / t;
	r.y = v.y / t;
	r.z = v.z / t;

	return r;
}

float dot(pgn::Float3& a, pgn::Float3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float len(pgn::Float3& v)
{
	return sqrt(dot(v, v));
}

#pragma managed(push, off)
void initViewMat(float minFov)
{
	center = (aabb.min + aabb.max) / 2.0f;

	float r = aabb.min.x > aabb.max.x || aabb.min.y > aabb.max.y || aabb.min.z > aabb.max.z ? 0 : len(center - aabb.min);
	float dist = r / sin(minFov / 2.0f);

	viewMat[0][0] = 1.0f;		viewMat[1][0] = 0.0f;		viewMat[2][0] = 0.0f;
	viewMat[0][1] = 0.0f;		viewMat[1][1] = 1.0f;		viewMat[2][1] = 0.0f;
	viewMat[0][2] = 0.0f;		viewMat[1][2] = 0.0f;		viewMat[2][2] = 1.0f;
	viewMat[0][3] = -center.x;	viewMat[1][3] = -center.y;	viewMat[2][3] = -center.z + dist;
}
#pragma managed(pop)

#pragma managed(push, off)
void copy(pgn::Float4x3& dest, pgn::Float4x3& src)
{
	dest = src;
}
#pragma managed(pop)

std::string EntityView::getFullName(String^ fileName)
{
	using namespace System::Runtime::InteropServices;

	String^ fullName;

	if (System::IO::Path::IsPathRooted(fileName))
		fullName = fileName;
	else
		fullName = options->tempDir + fileName;

	char* cstr;
	std::string str;

	cstr = (char*)(void*)Marshal::StringToHGlobalAnsi(fullName);
	str = cstr;
	Marshal::FreeHGlobal(System::IntPtr((void*)cstr));

	return str;
}

void EntityView::update(EntityProperties^ props, DirtyFlags^ dirtyFlags)
{
	float aspectRatio = (float)wnd->getClientWidth() / (float)wnd->getClientHeight();
	float vFov = options->verticalFovFixed ? options->fov : options->fov / aspectRatio;

	camera->setFrustumFovLH(vFov, aspectRatio, 0.01f, 1024);
	camera->setViewport(0, 0, wnd->getClientWidth(), wnd->getClientHeight(), wnd->getClientHeight());

	dirLight->setIntensity(1.0f, 1.0f, 1.0f, 1.0f);
	sceneDirLight->setDirection(&dir1);

	if (dirtyFlags->mesh)
	{
		model->setMesh(getFullName(props->mesh).c_str());

		if (props->diffuseMap == "")
		{
			model->setDiffuseMap(0, "gray");
		}
		else
		{
			model->setDiffuseMap(0, getFullName(Utilities::getFileName(props->diffuseMap, ".TGA") + ".dxt.PNT").c_str());
		}

		viewMatDirty = true;
	}

	if (dirtyFlags->diffuseMap)
	{
		if (props->diffuseMap == "")
		{
			model->setDiffuseMap(0, "gray");
		}
		else
		{
			model->setDiffuseMap(0, getFullName(Utilities::getFileName(props->diffuseMap, ".TGA") + ".dxt.PNT").c_str());
		}
	}

	if (dirtyFlags->anims)
	{
		skel->stopAll();

		std::map<std::string, pgn::Animation*> tmp;

		for each(Anim^ animProp in props->anims)
		{
			pgn::Animation* anim = animFactory->createAnimation();
			std::string fullName = getFullName(animProp->fileName);

			anim->set(fullName.c_str());
			tmp[fullName.c_str()] = anim;
		}

		for (auto& entry : *anims)
			entry.second->destroy();

		*anims = tmp;
	}

	if (dirtyFlags->navMesh)
	{
		navModel->setMesh(getFullName(props->navMesh).c_str());
		viewMatDirty = true;
	}

	if (viewMatDirty)
	{
		viewMatDirty = false;

		aabb.init();
		pgn::Float3* min = new pgn::Float3;
		pgn::Float3* max = new pgn::Float3;

		if (props->mesh != "")
		{
			if (model->complete())
			{
				model->getAabb(0, min, max);
				aabb.add(min);
				aabb.add(max);
			}
			else
			{
				viewMatDirty = true;
			}
		}

		if (props->navMesh != "")
		{
			if (navModel->complete())
			{
				navModel->getAabb(min, max);
				aabb.add(min);
				aabb.add(max);
			}
			else
			{
				viewMatDirty = true;
			}
		}

		delete min;
		delete max;

		float hFov = vFov * aspectRatio;
		initViewMat(min(vFov, hFov));
	}

	if (dirtyFlags->curAnim)
	{
		for each (Object^ o in options->curAnims)
		{
			Anim^ animProp = (Anim^)o;

			skel->playAnimation((*anims)[getFullName(animProp->fileName)]);
		}
	}

	int _t = (int)clock->getTickCount();
	int dt = _t - t;
	t = _t;

	int animDt = dt;
	skel->updatePose(animDt);

	sceneEntity->setWorldMat(&worldMat);
	navModelSceneEntity->setWorldMat(&worldMat);

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

		pgn::calculateInverseViewMat(&tempViewMat, &invViewMat);
		skyBoxWorldMat[0][3] = invViewMat[0][3];
		skyBoxWorldMat[1][3] = invViewMat[1][3];
		skyBoxWorldMat[2][3] = invViewMat[2][3];
		sceneSkyBox->setWorldMat(&skyBoxWorldMat);
	}
	else if (draggingGestureRecognizer->getState() == pgn::gestureEnded)
	{
		copy(viewMat, tempViewMat);
	}
	else
	{
		camera->setViewMat(&viewMat);

		pgn::calculateInverseViewMat(&viewMat, &invViewMat);
		skyBoxWorldMat[0][3] = invViewMat[0][3];
		skyBoxWorldMat[1][3] = invViewMat[1][3];
		skyBoxWorldMat[2][3] = invViewMat[2][3];
		sceneSkyBox->setWorldMat(&skyBoxWorldMat);
	}

	if (graphics->beginFrame())
	{
		graphics->draw(scene, camera);
		graphics->endFrame();
	}
}
