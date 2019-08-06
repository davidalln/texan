#pragma once

#include "deck.h"
#include "equity.h"

MU_TEST(simple_equity_test) {
	deck_t * deck = k_newDeck();
	card_t * community = (card_t *)malloc(sizeof(card_t) * 5);
	community[0] = d_newCard(ACE, SPADE);
	community[1] = d_newCard(SEVEN, CLUB);
	community[2] = d_newCard(TWO, DIAMOND);
	community[3] = d_newNullCard();
	community[4] = d_newNullCard();
	hand_t hero = h_newHand(d_newCard(EIGHT, SPADE), d_newCard(SIX, SPADE));
	hand_t villain = h_newHand(d_newCard(ACE, HEART), d_newCard(KING, HEART));

	k_removeCards(deck, community, 3);
	k_removeCards(deck, hero.cards, 2);
	k_removeCards(deck, villain.cards, 2);

	char hstr[10];
	char vstr[10];
	char c0str[10];
	char c1str[10];
	char c2str[10];
	char c3str[10];
	char c4str[10];
	h_toString(hero, hstr);
	h_toString(villain, vstr);
	d_toString(community[0], c0str);
	d_toString(community[1], c1str);
	d_toString(community[2], c2str);
	d_toString(community[3], c3str);
	//d_toString(community[4], c4str);

	float equity = q_calculateHandVsHandEquity(hero, villain, deck, community, 3);
	printf("hero [%s] vs villain [%s] on board %s%s%s%s has equity %%%.2f\n", hstr, vstr, c0str, c1str, c2str, c3str, equity * 100.f);

	free(deck);
	free(community);
}

MU_TEST(range_equity_test) {
	deck_t * deck = k_newDeck();
	card_t * community = (card_t *)malloc(sizeof(card_t) * 5);
	community[0] = d_newCard(ACE, SPADE);
	community[1] = d_newCard(EIGHT, CLUB);
	community[2] = d_newCard(TWO, DIAMOND);
	community[3] = d_newCard(FIVE, CLUB);
	community[4] = d_newCard(TEN, DIAMOND);
	hand_t hero = h_newHand(d_newCard(EIGHT, SPADE), d_newCard(SIX, SPADE));

	k_removeCards(deck, community, 3);
	k_removeCards(deck, hero.cards, 2);

	range_t * range = r_newRange();

	combo_t combos[5];
	combos[0] = c_newCombo(ACE, QUEEN);
	combos[1] = c_newCombo(QUEEN, JACK);
	combos[2] = c_newCombo(JACK, QUEEN);
	combos[3] = c_newCombo(QUEEN, TEN);
	combos[4] = c_newCombo(TWO, TWO);

	r_addCombo(range, combos[2]);
	r_addCombo(range, combos[1]);
	r_addCombo(range, combos[4]);
	r_addCombo(range, combos[0]);
	r_addCombo(range, combos[3]);

	r_deleteCards(range, community, 3);

	float equity = q_calculateHandVsRangeEquity(hero, range, deck, community, 3);

	r_deleteRange(range);

	free(deck);
	free(community);
}

MU_TEST_SUITE(equity_test_suite) {
	//MU_RUN_TEST(simple_equity_test);
	MU_RUN_TEST(range_equity_test);
}