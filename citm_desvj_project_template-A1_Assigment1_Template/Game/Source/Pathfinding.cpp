#include "App.h"
#include "Map.h"
#include "PathFinding.h"

#include "Defs.h"
#include "Log.h"

PathFinding::PathFinding()
{
	map = NULL;
	width = 0;
	height = 0;
}

// Destructor
PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	lastPath.Clear();
	RELEASE_ARRAY(map);

	return true;
}

// Sets up the navigation map
void PathFinding::SetNavigationMap(uint w, uint h, uchar* data)
{
	width = w;
	height = h;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

// Utility: return true if pos is inside the map boundaries
bool PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar walkId = GetTileAt(pos);
	bool isWalkable = walkId != INVALID_WALK_CODE && walkId > 0;
	return  isWalkable;
}

// Utility: return the walkability value of a tile
uchar PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const DynArray<iPoint>* PathFinding::GetLastPath() const
{
	return &lastPath;
}

// To request all tiles involved in the last generated path
void PathFinding::ClearLastPath()
{
	lastPath.Clear();
}


// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	int ret = -1;

	// L13: TODO 1: if origin or destination are not walkable, return -1
	if (IsWalkable(origin) && IsWalkable(destination)) {

		// L13: TODO 2: Create two lists: frontier, visited
		PathList frontier;
		PathList visited;

		// Create a PathNode with the origin position and add it to frontier list
		frontier.list.Add(PathNode(0, 0, origin, NULL));

		// Iterate while we have node in the frontier list
		while (frontier.list.Count() > 0) {

			// L13: TODO 3: Get the lowest score cell from frontier list and delete it from the frontier list
			// Keep a reference to the node before deleting the node from the list
			ListItem<PathNode>* lowest = frontier.GetNodeLowestScore();
			ListItem<PathNode>* node = new ListItem<PathNode>(*lowest);
			frontier.list.Del(lowest);

			// L13: TODO 6: If we just added the destination, we are done!
			if (node->data.pos == destination) {
				// Backtrack to create the final path
					// Use the Pathnode::parent and Flip() the path when you are finish
				lastPath.Clear();
				const PathNode* pathNode = &node->data;
				while (pathNode != NULL) {
					lastPath.PushBack(pathNode->pos);
					pathNode = pathNode->parent;
				}

				lastPath.Flip();
				ret = lastPath.Count();
				break;


			}

			// L13: TODO 4: Fill a list of all adjancent nodes. 
			// use the FindWalkableAdjacents function
			PathList adjacent;
			node->data.FindWalkableAdjacents(adjacent);

			// L13: TODO 5: Iterate adjancent nodes:
			ListItem<PathNode>* neigbourg = adjacent.list.start;
			while (neigbourg != NULL) {
				// ignore the nodes already in the visited list
				if (visited.Find(neigbourg->data.pos) == NULL) {
					// if is not in the visited lits
					// add the neighbourg to the visited list
					neigbourg->data.CalculateF(destination);
					frontier.list.Add(neigbourg->data);
					visited.list.Add(neigbourg->data);

					// If the neighbourgh is NOT found in the frontier list, calculate its F and add it to the frontier list
					// If it is already in the frontier list, check if it is a better path (compare G)
				}
				neigbourg = neigbourg->next;
			}
		}
	}
	return ret;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return item;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}

	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& listToFill) const
{
	iPoint tile;
	uint before = listToFill.list.Count();

	// top
	tile.Create(pos.x, pos.y + 1);
	if (app->map->pathfinding->IsWalkable(tile)) listToFill.list.Add(PathNode(-1, -1, tile, this));

	// bottom
	tile.Create(pos.x, pos.y - 1);
	if (app->map->pathfinding->IsWalkable(tile)) listToFill.list.Add(PathNode(-1, -1, tile, this));

	// left
	tile.Create(pos.x + 1, pos.y);
	if (app->map->pathfinding->IsWalkable(tile)) listToFill.list.Add(PathNode(-1, -1, tile, this));

	// right
	tile.Create(pos.x - 1, pos.y);
	if (app->map->pathfinding->IsWalkable(tile)) listToFill.list.Add(PathNode(-1, -1, tile, this));

	return listToFill.list.Count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceNoSqrt(destination);

	return g + h;
}
