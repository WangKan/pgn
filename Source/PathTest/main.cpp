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
#include <PGN/Math/Utilities.h>
#include <ode/ode.h>
#include "SceneManager.h"
#include "PathFinder.h"

void buildViewMatrix(pgn::Float4x3& viewMatrix, pgn::Float3& eye, pgn::Float3& lookAt, pgn::Float3& up)
{
	pgn::Float3 d = normalize(lookAt - eye);

	// Calculate x
	pgn::Float3 r = normalize(cross(d, up));

	up = normalize(cross(r, d));

	// Fill in the view matrix entries
	viewMatrix[0][0] = r[0];
	viewMatrix[0][1] = r[1];
	viewMatrix[0][2] = r[2];

	viewMatrix[1][0] = up[0];
	viewMatrix[1][1] = up[1];
	viewMatrix[1][2] = up[2];

	viewMatrix[2][0] = -d[0];
	viewMatrix[2][1] = -d[1];
	viewMatrix[2][2] = -d[2];

	viewMatrix[0][3] = -dot(eye, r);
	viewMatrix[1][3] = -dot(eye, up);
	viewMatrix[2][3] = dot(eye, d);
}

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
	pgn::Float3 eye = { 0.0f, 10.0f, -40.0f };
	pgn::Float3 lookAt = { 0, 30, 0 };
	pgn::Float3 up = { 0, 1, 0 };
	buildViewMatrix(viewMat, eye, lookAt, up);
	sm->setCameraViewMat(viewMat);

	pgn::Float4x3 worldMat =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
	};
	sm->addModel("res/ground.pnm", worldMat, 1.0f);

	pgn::NavModel* model = sm->addNavModel("res/ground.nav");
	dInitODE2(0);
	dWorldID world = dWorldCreate();
	dSpaceID space = dSimpleSpaceCreate(0);
	dGeomID ground = 0;
	float groundHeight = 0.0f;
	PathFinder* pathFinder = debug_new PathFinder;	
	if (model != NULL)
	{
		dTriMeshDataID meshData = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildSingle(meshData, model->getVertexBuffer(), 3 * sizeof(float), model->getVertexCount(), 
			model->getIndexBuffer(), model->getIndexCount(), sizeof(unsigned short));
		ground = dCreateTriMesh(space, meshData, 0, 0, 0);
		dGeomID ray = dCreateRay(space, 100.f);
		dGeomRaySet(ray, -8.0f, 50.0f, -6.0f, 0.0f, -1.0f, 0.0f);
		dContactGeom c;
		int result = dCollide(ray, ground, 1, &c, sizeof(dContactGeom));
		if (result > 0)
			groundHeight = c.pos[1];

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
			pgn::Float3 begin, end;
			sm->screenPointToRay(begin, end, (float)pt.x, (float)pt.y);
			dGeomID ray = dCreateRay(space, 100.f);
			pgn::Float3 dir = normalize(end - begin);
			dGeomRaySet(ray, begin[0], begin[1], begin[2], dir[0], dir[1], dir[2]);
			dContactGeom c;
			int result = dCollide(ray, ground, 1, &c, sizeof(dContactGeom));
			if (result > 0 && pathFinder != NULL)
			{
				pgn::Float3 startPos;
				cha->getWorldPos(startPos);

				pgn::Float3 endPos = { c.pos[0], c.pos[1], c.pos[2] };
				pathFinder->search(startPos, endPos, path);
				
				tickCount = 0;
			}
		}
		tickCount++;
		if (!path.empty() && tickCount % 10000 == 0)
		{
			pgn::Float3 pos = path[0];
			dGeomID ray = dCreateRay(space, 100.f);
			dGeomRaySet(ray, pos[0], 50.0f, pos[2], 0.0f, -1.0f, 0.0f);
			dContactGeom c;
			int result = dCollide(ray, ground, 1, &c, sizeof(dContactGeom));
			if (result > 0)
				groundHeight = c.pos[1];

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
