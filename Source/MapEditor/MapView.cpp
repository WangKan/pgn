#include <PGN/FileStream/FileStream.h>
#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Graphics/Camera.h>
#include <PGN/Graphics/DirectionalLight.h>
#include <PGN/Graphics/EditableModel.h>
#include <PGN/Graphics/SkeletalModel.h>
#include <PGN/Graphics/Graphics.h>
#include <PGN/Graphics/Scene.h>
#include <PGN/Graphics/SceneDirectionalLight.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Graphics/SceneManager.h>
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
#include "MapView.h"

#pragma managed(push, off)
pgn::Float3 lightPos = { 15, 50, 0 };
pgn::Float4 lightIntensity = { 5.0f, 5.0f, 5.0f, 5.0f };
pgn::Float3 eye = { 0.0f, 30.0f, -20.0f };
pgn::Float3 lookAt = { 0, 0, 0 };
pgn::Float3 groundPos = { 0, 0, 0 };
pgn::Float3 chaPos = { -8.0f, 0.0f, -6.0f };
pgn::SceneEntity* cha = NULL;

pgn::Float4x3 chaWorldMat =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
};
pgn::Float3 center = {0.0f, 0.0f, 0.0f};
pgn::Float4x3 viewMat =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
};
pgn::Float4x3 tempViewMat;

void copy(pgn::Float4x3& dest, pgn::Float4x3& src)
{
	dest = src;
}
#pragma managed(pop)

MapView::MapView(IntPtr^ hWnd, Options^ options)
{
	this->options = options;
	_isDisposed = false;

	_beginDebugHeap();

	wnd = pgn::Window::create(8, 8, 8, 8, 24, 8, 1, (HWND)hWnd->ToPointer());

	clock = pgn::Clock::create(1, 4800);
	t = (int)clock->getTickCount();

	draggingGestureRecognizer = pgn::DraggingGestureRecognizer::create();
	wnd->addGestureRecognizer(draggingGestureRecognizer);

	sm = pgn::SceneManager::create();
	sm->init(wnd);
	sm->addPointLight(&lightIntensity, 100.0f, &lightPos);
	sm->setCamera(&eye, &lookAt);

	scnEntityDelList = gcnew List<String^>();
}

MapView::~MapView()
{
	wnd->destroy();

	sm->destroy();
	clock->destroy();
	draggingGestureRecognizer->destroy();
	_endDebugHeap();
	_isDisposed = true;
}

void MapView::update(MapProperties^ props)
{
	int _t = (int)clock->getTickCount();
	int dt = _t - t;
	t = _t;

	sm->tick(dt);

	int i = 0;
	for each (KeyValuePair<String^, SceneEntityProperties^> kvp in props->entities)
	{
		SceneEntityProperties^ entityProp = kvp.Value;

		if (entityProp->dirtyFlags->file)
		{
			char* name = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(entityProp->entityName);
			char* mesh = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(entityProp->props->mesh);
			sm->addCharacter(name, mesh, "gray", &groundPos, 1.0f);
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)mesh));
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)name));
		}

		if (entityProp->dirtyFlags->worldMat)
		{
			char* name = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(entityProp->entityName);
			cha = sm->getCharacter(name);
			if (cha != NULL)
			{
				chaWorldMat[0][3] = entityProp->position_x;
				chaWorldMat[1][3] = entityProp->position_y;
				chaWorldMat[2][3] = entityProp->position_z;
				chaWorldMat[0][0] = entityProp->scale_x;
				chaWorldMat[1][1] = entityProp->scale_y;
				chaWorldMat[2][2] = entityProp->scale_z;
				cha->setWorldMat(&chaWorldMat);
			}
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)name));
		}

		if (entityProp->dirtyFlags->del)
		{
			char* name = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(entityProp->entityName);
			//sm->delCharacter(name);
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)name));
			scnEntityDelList->Add(entityProp->entityName);
		}

		entityProp->dirtyFlags->clear();
	}

	for each (String^ name in scnEntityDelList)
	{
		props->entities->Remove(name);
	}
	scnEntityDelList->Clear();
}

void MapView::init()
{
}

void MapView::clear()
{
	sm->clear();
}

