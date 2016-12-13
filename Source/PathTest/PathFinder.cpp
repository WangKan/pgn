#include <PGN/Common/debug_new.h>
#include <PGN/Math/Utilities.h>
#include "PathFinder.h"

int Line::classifyPoint(pgn::Float3& p)
{
	pgn::Float3 ab;
	ab[0] = pointB[0] - pointA[0];
	ab[2] = pointB[2] - pointA[2];
	pgn::Float3 ap;
	ap[0] = p[0] - pointA[0];
	ap[2] = p[2] - pointA[2];
	return (int)(ab[0] * ap[2] - ap[0] * ab[2]);
}

int PathNode::getAdjacentSide(unsigned short a, unsigned short b)
{
	if ((a == indices[0] && b == indices[1]) || (a == indices[1] && b == indices[0]))
	{
		return 0;
	}

	if ((a == indices[1] && b == indices[2]) || (a == indices[2] && b == indices[1]))
	{
		return 1;
	}

	if ((a == indices[2] && b == indices[0]) || (a == indices[0] && b == indices[2]))
	{
		return 2;
	}
	return -1;
}

void PathNode::computeHeuristic(pgn::Float3& pos)
{
	h = length(center - pos);
}

void PathNode::setArrivalSide(int idx, PathNode* node)
{
	int v1 = idx;
	int v2 = idx + 1;
	if (v2 > 2)
		v2 = 0;
	arrivalSide = getAdjacentSide(node->indices[v1], node->indices[v2]);
}

float PathNode::getSideDistance(int side)
{
	if (arrivalSide == -1) return 0.0f;

	int diff = abs(side - arrivalSide);
	if (diff == 1)
	{
		if ((side == 0 && arrivalSide == 1) || (side == 1 && arrivalSide == 0))
			diff = 0;
	}
	return sideDistance[diff];
}

PathFinder::PathFinder() : sessionID(0)
{
	pathNodes.reserve(1024);
	openList.reserve(512);
	closeList.reserve(512);
}

PathFinder::~PathFinder()
{
	std::vector<PathNode*>::iterator it = pathNodes.begin();
	for (; it != pathNodes.end(); ++it)
	{
		delete (*it);
	}
}

void PathFinder::build(void* vertices, int vertexCount, void* indices, int indexCount, void* adjacentNodes)
{
	float* pVertices = (float*)vertices;
	unsigned short* pIndices = (unsigned short*)indices;
	unsigned short* pAdjacentNodes = (unsigned short*)adjacentNodes;
	for (int i = 0; i < indexCount; i+=3)
	{
		PathNode* node = debug_new PathNode;
		node->index = i / 3;
		node->indices[0] = pIndices[i];
		node->indices[1] = pIndices[i+1];
		node->indices[2] = pIndices[i+2];
		
		pgn::Float3 v[3];
		v[0] = { pVertices[node->indices[0] * 3], pVertices[node->indices[0] * 3 + 1], pVertices[node->indices[0] * 3 + 2] };
		v[1] = { pVertices[node->indices[1] * 3], pVertices[node->indices[1] * 3 + 1], pVertices[node->indices[1] * 3 + 2] };
		v[2] = { pVertices[node->indices[2] * 3], pVertices[node->indices[2] * 3 + 1], pVertices[node->indices[2] * 3 + 2] };

		node->vertices[0] = v[0];
		node->vertices[1] = v[1];
		node->vertices[2] = v[2];

		node->sides[0] = Line(v[0], v[1]);
		node->sides[1] = Line(v[1], v[2]);
		node->sides[2] = Line(v[2], v[0]);

		node->center[0] = (v[0][0] + v[1][0] + v[2][0]) / 3.0f;
		node->center[1] = (v[0][1] + v[1][1] + v[2][1]) / 3.0f;
		node->center[2] = (v[0][2] + v[1][2] + v[2][2]) / 3.0f;
		
		pgn::Float3 m[3];
		m[0] = { 0.5f*(v[0][0] + v[1][0]), 0.5f*(v[0][1] + v[1][1]), 0.5f*(v[0][2] + v[1][2]) };
		m[1] = { 0.5f*(v[1][0] + v[2][0]), 0.5f*(v[1][1] + v[2][1]), 0.5f*(v[1][2] + v[2][2]) };
		m[2] = { 0.5f*(v[2][0] + v[0][0]), 0.5f*(v[2][1] + v[0][1]), 0.5f*(v[2][2] + v[0][2]) };
		
		node->sideDistance[0] = length(m[0] - m[1]);
		node->sideDistance[1] = length(m[1] - m[2]);
		node->sideDistance[2] = length(m[2] - m[0]);
		
		pathNodes.push_back(node);
	}

	int maxFaceCount = indexCount / 3;
	for (size_t i = 0; i < pathNodes.size(); ++i)
	{
		PathNode* node = pathNodes[i];
		unsigned short index = pAdjacentNodes[i * 3];
		if (index < maxFaceCount)
			node->links[0] = pathNodes[index];

		index = pAdjacentNodes[i * 3 + 1];
		if (index < maxFaceCount)
			node->links[1] = pathNodes[index];
		
		index = pAdjacentNodes[i * 3 + 2];
		if (index < maxFaceCount)
			node->links[2] = pathNodes[index];
	}
}

bool compare(const PathNode* a, const PathNode* b)
{
	return (a->g + a->h) > (b->g + b->h);
}

bool PathFinder::search(pgn::Float3& start, pgn::Float3& end, std::deque<pgn::Float3>& path)
{
	sessionID++;
	openList.clear();
	closeList.clear();

	startPos = start;
	endPos = end;
	PathNode* startNode = getNodeFromPoint(start);
	PathNode* endNode = getNodeFromPoint(end);
	if (startNode == NULL || endNode == NULL)
		return false;

	endNode->g = 0;
	endNode->h = 0;
	endNode->open = false;
	endNode->parent = NULL;
	endNode->sessionID = sessionID;
	openList.push_back(endNode);

	bool foundPath = false;
	while (!openList.empty())
	{
		PathNode* currNode = openList.back();
		closeList.push_back(currNode);
		openList.pop_back();

		// 路径是在同一个三角形内
		if (currNode == startNode) 
		{
			foundPath = true;
			break;
		}

		for (int i = 0; i < 3; i++) 
		{
			PathNode* adjacentNode = currNode->links[i];
			// 相邻节点不可通行或该相邻节点已在封闭列表中，则什么操作也不执行，继续检验下一个节点
			if (adjacentNode == NULL) 
				continue;

			if (adjacentNode->sessionID != sessionID) 
			{
				// 相邻节点不在开放列表中，则将该节点添加到开放列表中，并将该相邻节点的父节点设为当前节点
				adjacentNode->sessionID = sessionID;
				adjacentNode->parent = currNode;
				adjacentNode->open = true;

				adjacentNode->g = currNode->g + currNode->getSideDistance(i);
				adjacentNode->setArrivalSide(i, currNode);
				adjacentNode->computeHeuristic(startPos);

				// 放入开放列表并排序
				openList.push_back(adjacentNode);
				std::sort(openList.begin(), openList.end(), compare);
			}
			else if (adjacentNode->open && (currNode->g + currNode->getSideDistance(i)) < adjacentNode->g)
			{
				// 如果该相邻节点在开放列表中，则判断经由当前节点到达该相邻节点的G值是否小于原来的G值，
				// 若小于，则将该相邻节点的父节点设为当前节点，并重设该相邻节点的G值
				adjacentNode->g = currNode->g;
				adjacentNode->parent = currNode;
				adjacentNode->setArrivalSide(i, currNode);
			}
		}
	}
	if (foundPath)
		getPath(path);
	return foundPath;
}

// 根据经过的三角形返回路径点（拐角点法）
void PathFinder::getPath(std::deque<pgn::Float3>& path)
{
	path.clear();
	std::vector<PathNode*> gridPath;
	getNavGridPath(gridPath);
	if (gridPath.empty())
		return;

	path.push_back(startPos);
	if (gridPath.size() == 1)
	{
		path.push_back(endPos);
		return;
	}

	// 获取路点
	WayPoint wayPoint(gridPath[0], startPos);
	while (wayPoint.pos != endPos)
	{
		getFurthestWayPoint(wayPoint, gridPath);
		path.push_back(wayPoint.pos);
	}

	//for (size_t i = 0; i < gridPath.size(); ++i)
	//{
	//	PathNode* node = gridPath[i];
	//	pgn::Float3 pos = { node->center[0], node->center[1], node->center[2] };
	//	path.push_back(pos);
	//}
	//path.push_back(endPos);
}

// 获取路径经过的三角形
void PathFinder::getNavGridPath(std::vector<PathNode*>& path)
{
	PathNode* node = closeList[closeList.size() - 1];
	path.push_back(node);

	while (node->parent != NULL) {
		path.push_back(node->parent);
		node = node->parent;
	}
}

void PathFinder::getFurthestWayPoint(WayPoint& wayPoint, std::vector<PathNode*>& gridPath)
{
	PathNode* lastNodeA = wayPoint.node;
	PathNode* lastNodeB = wayPoint.node;

	pgn::Float3 startPt = wayPoint.pos;
	Line outSide = wayPoint.node->sides[wayPoint.node->arrivalSide];
	pgn::Float3 lastPtA = outSide.pointA;
	pgn::Float3 lastPtB = outSide.pointB;
	Line lastLineA = Line(startPt, lastPtA);
	Line lastLineB = Line(startPt, lastPtB);
	pgn::Float3 testPtA, testPtB;
	std::vector<PathNode*>::iterator it = std::find(gridPath.begin(), gridPath.end(), wayPoint.node);
	it++;
	for(; it != gridPath.end(); ++it)
	{
		PathNode* node = *it;
		outSide = node->sides[node->arrivalSide];
		if (it == gridPath.end() - 1)
		{
			testPtA = endPos;
			testPtB = endPos;
		}else
		{
			testPtA = outSide.pointA;
			testPtB = outSide.pointB;
		}

		if (lastPtA != testPtA) 
		{
			if (lastLineB.classifyPoint(testPtA) > 0)
			{// RIGHT SIDE
				wayPoint = WayPoint(lastNodeB, lastPtB);
				return;
			}
			else if (lastLineA.classifyPoint(testPtA) >= 0)
			{
				lastPtA = testPtA;
				lastNodeA = node;
				lastLineA.pointB = lastPtA;
				lastLineB.pointB = lastPtB;
			}
		}

		if (lastPtB != testPtB) 
		{
			if (lastLineA.classifyPoint(testPtB) < 0) 
			{// LEFT SIDE
				wayPoint = WayPoint(lastNodeA, lastPtA);
				return;
			}
			else if (lastLineB.classifyPoint(testPtB) <= 0)
			{
				lastPtB = testPtB;
				lastNodeB = node;
				lastLineA.pointB = lastPtA;
				lastLineB.pointB = lastPtB;
			}
		}
	}
	wayPoint = WayPoint(gridPath[gridPath.size()-1], endPos);
}

PathNode* PathFinder::getNodeFromPoint(pgn::Float3& pos)
{
	for (size_t i = 0; i < pathNodes.size(); ++i)
	{
		PathNode* node = pathNodes[i];
		if (pgn::pointInTriangle(pos, node->vertices[0], node->vertices[1], node->vertices[2]))
		{
			return node;
		}
	}
	return NULL;
}

