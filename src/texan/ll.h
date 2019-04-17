#pragma once

typedef unsigned long data;

typedef struct ll_node ll_node_t;
struct ll_node {
	data payload;

	ll_node_t * next;
	ll_node_t * prev;
};

typedef struct {
	void(*decode)(data,void*);
	signed(*compare)(data,data);

	unsigned length;
	ll_node_t * head;
	ll_node_t * tail;
	ll_node_t * ptr;
} ll_t;

ll_t ll_newList();
void ll_deleteList(ll_t ll);
unsigned ll_isNull();

ll_t ll_search(ll_t ll, data payload);
ll_t ll_add(ll_t ll, data payload);
ll_t ll_delete(ll_t ll);
ll_t ll_head(ll_t ll);
ll_t ll_next(ll_t ll);
ll_t ll_prev(ll_t ll);
unsigned ll_atHead(ll_t ll);
unsigned ll_atTail(ll_t ll);
void ll_get(ll_t ll, void* get);

