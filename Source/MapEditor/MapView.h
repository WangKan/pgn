#pragma once

#include "EntityProperties.h"
#include "MapProperties.h"
#include "SceneEntityProperties.h"

namespace pgn {
	class Animation;
	class AnimationFactory;
	class Camera;
	class Clock;
	class DirectionalLight;
	class DraggingGestureRecognizer;
	class Entity;
	class FileStream;
	class Graphics;
	class Model;
	class PathFinder;
	class PathFinderFactory;
	class Scene;
	class SceneDirectionalLight;
	class SceneEntity;
	class SceneManager;
	class Skeleton;
	class SkeletonFactory;
	class Window;
}

public ref class MapView
{
	bool _isDisposed;
	int t;
	int lastTickCount;
	Options^ options;
	pgn::Clock* clock;
	pgn::Window* wnd;
	pgn::DraggingGestureRecognizer* draggingGestureRecognizer;
	pgn::SceneManager* sm;
	List<String^>^ scnEntityDelList;

public:
	MapView(IntPtr^ hWnd, Options^ options);
	~MapView();

	property bool isDisposed
	{
		bool get() { return _isDisposed; }
	}

	void update(MapProperties^ props);
	void init();
	void clear();
};

