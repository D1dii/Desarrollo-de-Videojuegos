#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"
#include "Pathfinding.h"

#include "PugiXml\src\pugixml.hpp"

// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tilewidth;
	int	tileheight;
	int columns;
	int tilecount;

	SDL_Texture* texture;
	// L06: DONE 7: Implement the method that receives the gid and returns a Rect

	SDL_Rect GetRect(uint gid) {
		SDL_Rect rect = { 0 };

		int relativeIndex = gid - firstgid;
		rect.w = tilewidth;
		rect.h = tileheight;
		rect.x = margin + (tilewidth + spacing) * (relativeIndex % columns);
		rect.y = margin + (tileheight + spacing) * (relativeIndex / columns);

		return rect;
	}
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	Property* GetProperty(const char* name);

	List<Property*> list;
};

struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	uint* tiles;

	Properties properties;

	MapLayer() : tiles(NULL)
	{}

	~MapLayer()
	{
		RELEASE(tiles);
	}

	inline uint Get(int x, int y) const
	{
		return tiles[(y * width) + x];
	}
};

struct MapData
{
	int width;
	int	height;
	int	tilewidth;
	int	tileheight;
	List<TileSet*> tilesets;
	MapTypes type;

	List<MapLayer*> maplayers;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
	bool Load(SString mapFileName);

	iPoint MapToWorld(int x, int y) const;
	iPoint Map::WorldToMap(int x, int y);

	TileSet* GetTilesetFromTileId(int gid) const;
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	// L13: Create navigation map for pathfinding
	void CreateNavigationMap(int& width, int& height, uchar** buffer) const;

	int GetTileWidth();
	int GetTileHeight();

private:

	bool LoadObjectGroups(pugi::xml_node MapNode);

public: 

	MapData mapData;
	SString name;
	SString path;
	PathFinding* pathfinding;

private:

	bool mapLoaded;
	MapLayer* navigationLayer;
	int blockedGid = 546;
};

#endif // __MAP_H__