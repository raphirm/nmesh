// Roles that a node can play
#define HOP    0
#define ZIEL   1
#define QUELLE 2
//Default TCP Port, will give a error
#define NO_TCP_PORT -1
//Size of Message, that comes in
#define MSIZE 132
//Size, how big the Buffer of consume thread
#define MAX_BUF_SIZE 100
//Defines how big the package list is
#define PACKAGE_LIST_MAX_ITEMS 128;

struct  paket
{
 unsigned short paketID;
 unsigned char	 target;
 char paketType;
 unsigned char content[MSIZE -4 ];
};
struct newp
{
 unsigned char ip[4];
 unsigned short port;
 char cont[MSIZE - 10];
};



struct route
{
	llist_node_t *zielt;
	llist_node_t *quellet;
};




struct threadinfos
{
	llist_t *nodes;
	llist_node_t *me;
	struct packetList *packages;
	struct route *routes;
	short node_role;
	
};
