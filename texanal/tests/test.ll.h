#pragma once

#include "ll.h"

static void returnInt(data payload, void* i) {
	*((int *)i) = (int)(payload);
}

static signed compareInt(data a, data b) {
	int ints[2];
	returnInt(a, ints);
	returnInt(b, ints + 1);

	if (a < b) {
		return 1;
	} else if (a > b) {
		return -1;
	} else {
		return 0;
	}
}

MU_TEST(ll_initialize) {
	ll_t list = ll_newList();
	mu_check(ll_isNull(list));
	ll_deleteList(list);
}

MU_TEST(ll_insert_one) {
	ll_t list = ll_newList();
	list.decode = returnInt;

	list = ll_add(list, 100);
	mu_assert_int_eq(1, list.length);
	ll_deleteList(list);
}

MU_TEST(ll_get_one) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	list = ll_add(list, 100);
	int get;
	ll_get(list, &get);

	mu_assert_int_eq(100, get);
	ll_deleteList(list);
}

MU_TEST(ll_insert_two) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	list = ll_add(list, 100);
	list = ll_add(list, 200);

	mu_assert_int_eq(2, list.length);
	ll_deleteList(list);
}

MU_TEST(ll_get_two) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	list = ll_add(list, 100);
	list = ll_add(list, 200);

	int data;
	list = ll_head(list);
	ll_get(list, &data);
	mu_assert_int_eq(100, data);

	list = ll_next(list);
	ll_get(list, &data);
	mu_assert_int_eq(200, data);

	ll_deleteList(list);
}

MU_TEST(ll_get_five) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	int elems[5] = {10, 20, 30, 40, 50};

	for (int i = 0; i < 5; i++) {
		list = ll_add(list, elems[i]);
	}

	mu_assert_int_eq(5, list.length);

	int index = 0;
	list = ll_head(list);
	do {
		int data;
		ll_get(list, &data);
		mu_assert_int_eq(elems[index], data);
		list = ll_next(list);
		index++;
	} while (!ll_atHead(list));

	ll_deleteList(list);
}

MU_TEST(ll_get_five_reverse) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	int elems[5] = {10, 20, 30, 40, 50};

	for (int i = 0; i < 5; i++) {
		list = ll_add(list, elems[4 - i]);
	}

	mu_assert_int_eq(5, list.length);

	int index = 0;
	list = ll_head(list);
	do {
		int data;
		ll_get(list, &data);
		mu_assert_int_eq(elems[index], data);
		list = ll_next(list);
		index++;
	} while (!ll_atHead(list));

	ll_deleteList(list);
}

MU_TEST(ll_get_five_out_of_order) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	int elems[5] = {10, 20, 30, 40, 50};

	list = ll_add(list, elems[3]);
	list = ll_add(list, elems[4]);
	list = ll_add(list, elems[2]);
	list = ll_add(list, elems[0]);
	list = ll_add(list, elems[1]);

	mu_assert_int_eq(5, list.length);

	int index = 0;
	list = ll_head(list);
	do {
		int data;
		ll_get(list, &data);
		mu_assert_int_eq(elems[index], data);
		list = ll_next(list);
		index++;
	} while (!ll_atHead(list));

	ll_deleteList(list);
}

MU_TEST(ll_delete_one) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	list = ll_add(list, 100);
	mu_assert_int_eq(1, list.length);
	list = ll_delete(list);
	mu_assert_int_eq(0, list.length);
	mu_check(ll_isNull(list));

	ll_deleteList(list);
}

MU_TEST(ll_delete_one_with_remainder) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	list = ll_add(list, 100);
	list = ll_add(list, 200);
	mu_assert_int_eq(2, list.length);

	list = ll_delete(ll_head(list));
	mu_assert_int_eq(1, list.length);

	int data;
	ll_get(list, &data);
	mu_assert_int_eq(200, data);

	ll_deleteList(list);
}

MU_TEST(ll_search_and_delete) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	int elems[5] = {10, 20, 30, 40, 50};
	list = ll_add(list, elems[1]);
	list = ll_add(list, elems[0]);
	list = ll_add(list, elems[2]);
	list = ll_add(list, elems[4]);
	list = ll_add(list, elems[3]);

	mu_assert_int_eq(5, list.length);

	int index = 0;
	list = ll_head(list);
	do {
		int data;
		ll_get(list, &data);
		mu_assert_int_eq(elems[index], data);
		list = ll_next(list);
		index++;
	} while (!ll_atHead(list));

	list = ll_delete(ll_search(list, 30));
	mu_assert_int_eq(4, list.length);

	index = 0;
	list = ll_head(list);
	do {
		if (elems[index] != 30) {
			int data;
			ll_get(list, &data);
			mu_assert_int_eq(elems[index], data);
			list = ll_next(list);
		}
		index++;
	} while (!ll_atHead(list));

	ll_deleteList(list);
}

MU_TEST(ll_search_delete_search) {
	ll_t list = ll_newList();
	list.decode = returnInt;
	list.compare = compareInt;

	int elems[5] = {10, 20, 30, 40, 50};
	list = ll_add(list, elems[1]);
	list = ll_add(list, elems[0]);
	list = ll_add(list, elems[2]);
	list = ll_add(list, elems[4]);
	list = ll_add(list, elems[3]);

	mu_assert_int_eq(5, list.length);

	list = ll_delete(ll_search(list, 30));
	mu_assert_int_eq(4, list.length);


	ll_node_t * ptr = list.ptr;
	list = ll_search(list, 30);
	mu_check(ptr == list.ptr);
}


MU_TEST_SUITE(linked_list_suite) {
	MU_RUN_TEST(ll_initialize);
	MU_RUN_TEST(ll_insert_one);
	MU_RUN_TEST(ll_get_one);
	MU_RUN_TEST(ll_insert_two);
	MU_RUN_TEST(ll_get_two);
	MU_RUN_TEST(ll_get_five);
	MU_RUN_TEST(ll_get_five_reverse);
	MU_RUN_TEST(ll_get_five_out_of_order);
	MU_RUN_TEST(ll_delete_one);
	MU_RUN_TEST(ll_delete_one_with_remainder);
	MU_RUN_TEST(ll_search_and_delete);
	MU_RUN_TEST(ll_search_delete_search);
}
