#include <stdio.h>
#include <stdlib.h>

#include "ll.h"

ll_t ll_newList() {
	ll_t list;
	list.decode = NULL;
	list.compare = NULL;

	list.length = 0;
	list.head = NULL;
	list.ptr = NULL;	

	return list;
}

void ll_deleteList(ll_t ll) {
	if (!ll_isNull(ll)) {
		ll_node_t * start = ll.ptr;
		do {
			ll_node_t * next = ll.ptr->next;
			free(ll.ptr);
			ll.ptr = next;
		} while (ll.ptr != start);
	}

	ll.head = NULL;
	ll.ptr = NULL;
}

unsigned ll_isNull(ll_t ll) {
	return (ll.ptr == NULL && ll.head == NULL);
}

ll_t ll_search(ll_t ll, data payload) {
	if (ll_isNull(ll)) {
		return ll;
	} else {
		ll_node_t * start = ll.ptr;
		//printf("\tsearch started...\n");
		do {
			//printf("\t\tsearch at payload %d\n", ll.ptr->payload);
			signed cmp_current = (*ll.compare)(ll.ptr->payload, payload);
			signed cmp_next = (*ll.compare)(payload, ll.ptr->next->payload);

			if (cmp_current > 0) {
				if (cmp_next > 0 || (ll.ptr->next == ll.head && cmp_next < 0)) {
					//printf("a");
					return ll;
				}
				ll.ptr = ll.ptr->next;
			} else if (cmp_current < 0) {
				if (ll.ptr->next == ll.head && cmp_next > 0) {
					//printf("b");
					return ll;
				}
				ll.ptr = ll.ptr->prev;
			} else {
				return ll;
			}
		} while (ll.ptr != start);
		//printf("\tsearch set pointer to payload %d\n", ll.ptr->payload);
	}

	return ll;
}

ll_t ll_add(ll_t ll, data payload) {
	if (ll_isNull(ll)) {
		//printf("adding payload %d (initial)...\n", payload);
		ll.ptr = (ll_node_t *)(malloc(sizeof(ll_node_t)));
		if (!ll.ptr) return ll;

		ll.ptr->next = ll.ptr;
		ll.ptr->prev = ll.ptr;
		ll.ptr->payload = payload;
		ll.head = ll.ptr;

		ll.length = 1;
	} else {
		//printf("adding payload %d by searching...\n", payload);
		ll = ll_search(ll, payload);
		
		ll_node_t * new = (ll_node_t *)(malloc(sizeof(ll_node_t)));

		new->payload = payload;
		new->prev = ll.ptr;
		new->next = ll.ptr->next;
		new->next->prev = new;

		if ((*ll.compare)(payload, ll.head->payload) >= 0) {
			ll.head = new;
		}

		if (ll.ptr->prev == ll.ptr) {
			ll.ptr->prev = new;
		}

		ll.ptr->next = new;
		ll.ptr = new;
		ll.length++;
	}

	return ll;
}

ll_t ll_delete(ll_t ll) {
	if (!ll_isNull(ll)) {
		ll_node_t * ptr = ll.ptr;
		ptr->prev->next = ptr->next;
		ptr->next->prev = ptr->prev;

		ll.ptr = ptr->prev;

		if (ll.ptr == ll.head)
			ll.head = ptr->next;

		ll.length--;

		free(ptr);

		if (ll.length == 0) {
			ll.head = NULL;
			ll.ptr = NULL;
		}
	}

	return ll;
}

ll_t ll_head(ll_t ll) {
	ll.ptr = ll.head;
	return ll;
}

ll_t ll_next(ll_t ll) {
	if (!ll_isNull(ll)) {
		ll.ptr = ll.ptr->next;
	}

	return ll;
}

ll_t ll_prev(ll_t ll) {
	if (!ll_isNull(ll)) {
		ll.ptr = ll.ptr->prev;
	}

	return ll;
}

unsigned ll_atHead(ll_t ll) {
	return (ll.ptr == ll.head);
}

void ll_get(ll_t ll, void* get) {
	if (ll.decode) {
		return (*ll.decode)(ll.ptr->payload, get);
	} else {
		fprintf(stderr, "texanal linked list: trying to access a null list\n");
	}
}

