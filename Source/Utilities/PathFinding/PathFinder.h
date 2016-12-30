#pragma once
#include <PGN/Utilities/PathFinding/PathFinder.h>
#include <PGN/Math/Math.h>
#include <vector>
#include <queue>

namespace pgn {
	class ResourceHandle;
	class ResourceManager;
	class Pool;
	struct Float3;
}

struct Line
{
	pgn::Float3 pointA;
	pgn::Float3 pointB;

	Line()
	{
	}

	Line(pgn::Float3& a, pgn::Float3& b)
	{
		pointA = a;
		pointB = b;
	}

	int classifyPoint(pgn::Float3& p);
};

struct PathNode
{
	int sessionID;
	float g;
	float h;
	bool open;
	PathNode* parent;
	PathNode* links[3];
	int arrivalSide;

	int index;
	Line sides[3];
	float sideDistance[3];
	pgn::Float3 center;
	pgn::Float3 vertices[3];
	unsigned short indices[3];
	PathNode()
	{
		sessionID = 0;
		g = 0.0f;
		h = 0.0f;
		open = false;
		parent = NULL;
		links[0] = NULL;
		links[1] = NULL;
		links[2] = NULL;
		arrivalSide = -1;
	}

	int getAdjacentSide(unsigned short a, unsigned short b);
	void computeHeuristic(pgn::Float3& pos);
	void setArrivalSide(int idx, PathNode* node);
	float getSideDistance(int side);
};

struct WayPoint
{
	pgn::Float3 pos;
	PathNode* node;
	WayPoint(PathNode* n, const pgn::Float3& p)
	{
		node = n;
		pos = p;
	}
};

class PathFinder : public pgn::PathFinder
{
public:
	pgn::Pool* pool;
	pgn::ResourceManager* navMeshMgr;
	pgn::ResourceHandle* navMesh;
	bool _ready;

	PathFinder(pgn::Pool* pool, pgn::ResourceManager* navMeshMgr);
	virtual ~PathFinder();
	void init();
	virtual void dispose();
	virtual void _free();
	virtual void setNavigationMesh(char fileName[]);
	virtual bool ready();

	virtual void build();
	virtual bool search(pgn::Float3& start, pgn::Float3& end, std::deque<pgn::Float3>& path);
	PathNode* getNodeFromPoint(pgn::Float3& pos);

private:
	void getPath(std::deque<pgn::Float3>& path);
	void getNavGridPath(std::vector<PathNode*>& path);
	void getFurthestWayPoint(WayPoint& wayPoint, std::vector<PathNode*>& gridPath);

	unsigned int sessionID;
	pgn::Float3 startPos;
	pgn::Float3 endPos;
	std::vector<PathNode*> pathNodes;
	std::vector<PathNode*> openList;
	std::vector<PathNode*> closeList;
};
