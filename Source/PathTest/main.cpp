#include <PGN/Common/debug_new.h>
#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Graphics/Camera.h>
#include <PGN/Graphics/Entity.h>
#include <PGN/Graphics/Graphics.h>
#include <PGN/Graphics/NavModel.h>
#include <PGN/Graphics/PointLight.h>
#include <PGN/Graphics/Scene.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Graphics/ScenePointLight.h>
#include <PGN/Platform/DebugHeap.h>
#include <PGN/Platform/UI/Window.h>
#include <PGN/Utilities/SkeletalAnimation/Animation.h>
#include <PGN/Utilities/SkeletalAnimation/AnimationFactory.h>
#include <PGN/Utilities/SkeletalAnimation/Skeleton.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonFactory.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplate.h>
#include <PGN/Utilities/Clock.h>
#include <PGN/Utilities/LinearTransformations.h>
#include <PGN/Math/Math.h>
#include "SceneManager.h"
#include "PathFinder.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_beginDebugHeap();
	pgn::Clock* clock = pgn::Clock::create(1, 4800);
	int t = (int)clock->getTickCount();

	int width = 1280;
	int height = 720;
	int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
	int desktopHeight = GetSystemMetrics(SM_CYSCREEN);
	pgn::Window* wnd = pgn::Window::create(8, 8, 8, 8, 24, 8, 1, "", 0, 0, width, height);

	SceneManager* sm = debug_new SceneManager();
	sm->init(wnd);

	pgn::Float3 pos = { 15, 50, 0 };
	pgn::Float4 intensity = { 5.0f, 5.0f, 5.0f, 5.0f };
	sm->addPointLight(intensity, 100.0f, pos);

	pgn::Float4x3 viewMat =
	{
		1, 0, 0, 0,
		0, 1, 0, -40,
		0, 0, 1, 120,
	};
	pgn::Float3 eye = { 0.0f, 30.0f, -20.0f };
	pgn::Float3 lookAt = { 0, 0, 0 };
	pgn::Float3 up = { 0, 1, 0 };
	pgn::buildViewMat(&eye, &lookAt, &up, &viewMat);
	sm->setCameraViewMat(viewMat);

	pgn::Float4x3 worldMat =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
	};
	sm->addModel("res/ground.pnm", worldMat, 1.0f);

	pgn::NavModel* model = sm->addNavModel("res/ground.nav");
	float groundHeight = 0.0f;
	PathFinder* pathFinder = debug_new PathFinder;	
	if (model != NULL)
	{
		sm->addGroundToPhysicsWorld(model->getVertexBuffer(), model->getVertexCount(), model->getIndexBuffer(), model->getIndexCount());
		groundHeight = sm->getGroundHeight(-8.0f, -6.0f);

		pathFinder->build(model->getVertexBuffer(), model->getVertexCount(), 
			model->getIndexBuffer(), model->getIndexCount(), model->getAdjacentNodes());
	}

	pgn::Float4x3 chaWorldMat =
	{
		1, 0, 0, -8,
		0, 1, 0, groundHeight,
		0, 0, 1, -6,
	};
	pgn::SceneEntity* cha = sm->addCharacter("res/casual02_f_highpoly.pnm", chaWorldMat, 0.025f);

	std::deque<pgn::Float3> path;
	int tickCount = 0;
	while (wnd->processMessages())
	{
		int _t = (int)clock->getTickCount();
		int dt = _t - t;
		t = _t;
		sm->tick(dt);
		if (::GetAsyncKeyState(MK_LBUTTON))
		{
			POINT pt;
			GetCursorPos(&pt);
			pgn::Float3 begin, dir, contactPos;
			sm->screenPointToRay(pt.x, pt.y, begin, dir);
			if (sm->rayIntersectGround(begin, dir, contactPos) && pathFinder != NULL)
			{
				pgn::Float3 startPos;
				cha->getWorldPos(startPos);
				pathFinder->search(startPos, contactPos, path);
				tickCount = 0;
			}
		}
		tickCount++;
		if (!path.empty() && tickCount % 10000 == 0)
		{
			pgn::Float3 pos = path[0];
			float groundHeight = sm->getGroundHeight(pos[0], pos[2]);
			pgn::Float4x3 chaWorldMat =
			{
				1, 0, 0, pos[0],
				0, 1, 0, groundHeight,
				0, 0, 1, pos[2],
			};
			cha->setWorldMat(&chaWorldMat);
			path.pop_front();
		}
	}

	sm->destroy();
	delete sm;
	delete pathFinder;

	wnd->destroy();
	clock->destroy();
	_endDebugHeap();
}
