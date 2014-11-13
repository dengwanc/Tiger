#include <stdio.h>
#include "graph.h"
#include "liveness.h"

L_moveList L_MoveList(G_node src, G_node dst, L_moveList tail)
{
	L_moveList moves = checked_malloc(sizeof(*moves));
	moves->src = src;
	moves->dst = dst;
	moves->tail = tail;
	return moves;
}

struct L_graph L_liveness(G_graph flow)
{
	struct L_graph lg;
	return lg;
}

Temp_temp L_gtemp(G_node n)
{
}

static G_table LiveMap(G_graph flow)
{
	G_graph liveMap = G_empty();
	G_nodeList nl = flow->mynodes;
	for (; nl; nl = nl->tail) {
		G_enter(liveMap, nl->head, )
	}
}

static Temp_tempList exportLive(G_node n)
{
	Temp_tempList use = FG_use(n);
	G_nodeList nl;
	Temp_temp tmp;
}
