#include <PGN/Common/debug_new.h>
#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Graphics/SceneManager.h>
#include <PGN/Math/Math.h>
#include <PGN/Platform/DebugHeap.h>
#include <PGN/Platform/UI/Window.h>
#include <PGN/Utilities/PathFinding/PathFinder.h>
#include <PGN/Utilities/PathFinding/PathFinderFactory.h>
#include <PGN/Utilities/Physics/Physics.h>
#include <PGN/Utilities/Clock.h>

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
	pgn::FileStream* pathFinderStream = pgn::createStdFileStream("");
	pgn::PathFinderFactory* pathFinderFactory = pgn::PathFinderFactory::create(pathFinderStream);
	pgn::PathFinder* pathFinder = pathFinderFactory->createPathFinder();
	pathFinder->setNavigationMesh("res/ground.nav");

	pgn::SceneManager* sm = pgn::SceneManager::create();
	sm->init(wnd);

	pgn::Float3 lightPos = { 15, 50, 0 };
	pgn::Float4 lightIntensity = { 5.0f, 5.0f, 5.0f, 5.0f };
	sm->addPointLight(&lightIntensity, 100.0f, &lightPos);

	pgn::Float3 eye = { 0.0f, 30.0f, -20.0f };
	pgn::Float3 lookAt = { 0, 0, 0 };
	sm->setCamera(&eye, &lookAt);

	pgn::Float3 groundPos = { 0, 0, 0 };
	sm->addNavModel("res/ground.nav", &groundPos, 1.0f);
	pgn::Physics* ground = sm->addPhysics("res/ground.phy");
	while (true)
	{
		if (pathFinder->ready() && ground->complete())
		{
			sm->addGroundToPhysicsWorld(ground);
			pathFinder->build();
			break;
		}
	}

	float groundHeight = sm->getGroundHeight(-8.0f, -6.0f);
	pgn::Float3 chaPos = { -8.0f, groundHeight, -6.0f };
	pgn::SceneEntity* cha = sm->addCharacter("casual02_f_highpoly", "res/casual02_f_highpoly.pnm", "res/casual02_f_25.etc2.pnt", &chaPos, 0.025f);

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
			pgn::Float3 rayBegin, rayDir, contactPos;
			sm->screenPointToRay(pt.x, pt.y, &rayBegin, &rayDir);
			if (sm->rayIntersect(&rayBegin, &rayDir, &contactPos))
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
			float groundHeight = sm->getGroundHeight(pos.x, pos.z);
			pgn::Float4x3 chaWorldMat =
			{
				1, 0, 0, pos.x,
				0, 1, 0, groundHeight,
				0, 0, 1, pos.z,
			};
			cha->setWorldMat(&chaWorldMat);
			path.pop_front();
		}
	}
	sm->destroy();
	pathFinder->destroy();
	pathFinderFactory->destroy();
	destroyStdFileStream(pathFinderStream);
	wnd->destroy();
	clock->destroy();
	_endDebugHeap();
}
