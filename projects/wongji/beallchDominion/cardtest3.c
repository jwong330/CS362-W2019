#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NUMCARDS 10

/* Tests Adventurer card */
//Description:Adds up to 2 treasure cards from deck to player's hand.

void assertCheck(int left, int right, int * numError, char * validMessage, char * errMessage) {
	if (left != right) {
		printf("ERROR: %s\n", errMessage);
		*numError += 1;
	}
	else {
		printf("PASS: %s\n", validMessage);
	}
}

int main() {
	int seed = 100;
	int k[NUMCARDS] = { adventurer, council_room, feast, gardens, mine, remodel,
					smithy, village, baron, great_hall };
	struct gameState pre, G;
	int * bonus;
	int currentPlayer;
	int handPos;
	int cardInHand;
	int numError = 1;

	printf("----------Testing Adventurer Card----------\n");

	// -----------------Test Case 1-----------------------//
	printf("----------Test Case 1 - Treasure cards(2)----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Initialize game
	bonus = NULL;
	currentPlayer = whoseTurn(&G);
	handPos = G.handCount[currentPlayer];
	
	// Add adventurer to player hand
	G.hand[currentPlayer][handPos] = adventurer;
	G.handCount[currentPlayer]++;
	G.supplyCount[adventurer]--;
	
	pre = G;									// save game state before cardEffect() calls adventurer card
	assert(cardEffect(adventurer, copper, silver, gold, &G, handPos, bonus) == 0);	// play adventurer card

	// Check 2 cards added to hand from deck
	assertCheck(pre.handCount[currentPlayer] + 1, G.handCount[currentPlayer], &numError, "Hand count increased by 1.", "Incorrect hand count.");
	assertCheck(pre.deckCount[currentPlayer] - 2, G.deckCount[currentPlayer], &numError, "Deck count decreased by 2.", "Incorrect deck count.");
	// Check if those cards are treasure cards
	for (int location = 0; location < 2; location++) {
		int cardInHand = G.hand[currentPlayer][(G.handCount[currentPlayer] - 1) - location];
		switch (cardInHand) {
			case copper:
			case silver:
			case gold:
				break;
			default:
				numError++;
				printf("ERROR: Not a treasure card\n");
				break;
		}
	}

	// Check 2 cards should come from his own pile.
	// No state change should occur for other players.
	// No state change should occur to the victory card piles and kingdom card piles
	
	// Reverse changes from adventurer card
	memcpy(G.hand[currentPlayer], pre.hand[currentPlayer], sizeof(int) * MAX_HAND);
	G.handCount[currentPlayer] = pre.handCount[currentPlayer];
	memcpy(G.deck[currentPlayer], pre.deck[currentPlayer], sizeof(int) * MAX_DECK);
	G.deckCount[currentPlayer] = pre.deckCount[currentPlayer];

	// Reverse changes from discardCard()
	memcpy(G.playedCards, pre.playedCards, sizeof(int) * MAX_DECK);
	G.playedCardCount = pre.playedCardCount;
	memcpy(G.discard[currentPlayer], pre.discard[currentPlayer], sizeof(int) * MAX_DECK);
	G.discardCount[currentPlayer] = pre.discardCount[currentPlayer];

	// Check if changes made to other players
	assertCheck(memcmp(&pre, &G, sizeof(struct gameState)), 0, &numError, "No modifications to other gameState variables", "Other gameState variables were modified");


	// -----------------Test Case 2-----------------------//
	printf("\n----------Test Case 2 - Treasure cards(1)----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// RESET game
	bonus = NULL;
	currentPlayer = whoseTurn(&G);
	handPos = G.handCount[currentPlayer];

	// Add adventurer to player hand
	G.hand[currentPlayer][handPos] = adventurer;
	G.handCount[currentPlayer]++;
	G.supplyCount[adventurer]--;

	// 1 Treasure card in deck
	for (int i = 0; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = estate;
	}
	G.deck[currentPlayer][G.deckCount[currentPlayer] - 1] = copper;
	
	pre = G;									// save game state before cardEffect() calls adventurer card
	assert(cardEffect(adventurer, copper, silver, gold, &G, handPos, bonus) == 0);	// play adventurer card

	// Check 1 cards added to hand from deck
	assertCheck(pre.handCount[currentPlayer], G.handCount[currentPlayer], &numError, "Hand count remained the same.", "Incorrect hand count.");
	assertCheck(pre.deckCount[currentPlayer] - 1, G.deckCount[currentPlayer], &numError, "Deck count decreased by 1.", "Incorrect deck count.");

	// Check if that card is a treasure card
	cardInHand = G.hand[currentPlayer][G.handCount[currentPlayer] - 1];
	switch (cardInHand) {
		case copper:
		case silver:
		case gold:
			break;
		default:
			numError++;
			printf("ERROR: Not a treasure card\n");
			break;
	}

	// Check card should come from his own pile.
	// No state change should occur for other players.
	// No state change should occur to the victory card piles and kingdom card piles

	// Reverse changes from adventurer card
	memcpy(G.hand[currentPlayer], pre.hand[currentPlayer], sizeof(int) * MAX_HAND);
	G.handCount[currentPlayer] = pre.handCount[currentPlayer];
	memcpy(G.deck[currentPlayer], pre.deck[currentPlayer], sizeof(int) * MAX_DECK);
	G.deckCount[currentPlayer] = pre.deckCount[currentPlayer];

	// Reverse changes from discardCard()
	memcpy(G.playedCards, pre.playedCards, sizeof(int) * MAX_DECK);
	G.playedCardCount = pre.playedCardCount;
	memcpy(G.discard[currentPlayer], pre.discard[currentPlayer], sizeof(int) * MAX_DECK);
	G.discardCount[currentPlayer] = pre.discardCount[currentPlayer];

	// Check if changes made to other players
	assertCheck(memcmp(&pre, &G, sizeof(struct gameState)), 0, &numError, "No modifications to other gameState variables", "Other gameState variables were modified");


	// -----------------Test Case 3-----------------------//
	printf("\n----------Test Case 3 - Treasure cards(0)----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// RESET game
	bonus = NULL;
	currentPlayer = whoseTurn(&G);
	handPos = G.handCount[currentPlayer];

	// Add adventurer to player hand
	G.hand[currentPlayer][handPos] = adventurer;
	G.handCount[currentPlayer]++;
	G.supplyCount[adventurer]--;

	// 1 Treasure card in deck
	for (int i = 0; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = estate;
	}

	pre = G;									// save game state before cardEffect() calls adventurer card
	assert(cardEffect(adventurer, copper, silver, gold, &G, handPos, bonus) == 0);	// play adventurer card

	// Check if 0 cards added to hand from deck
	assertCheck(pre.handCount[currentPlayer] - 1, G.handCount[currentPlayer], &numError, "Hand count decreased by 1.", "Incorrect hand count.");
	assertCheck(pre.deckCount[currentPlayer], G.deckCount[currentPlayer], &numError, "Deck count remained the same.", "Incorrect deck count.");

	// Check card should come from his own pile.
	// No state change should occur for other players.
	// No state change should occur to the victory card piles and kingdom card piles

	// Reverse changes from adventurer card
	memcpy(G.hand[currentPlayer], pre.hand[currentPlayer], sizeof(int) * MAX_HAND);
	G.handCount[currentPlayer] = pre.handCount[currentPlayer];
	memcpy(G.deck[currentPlayer], pre.deck[currentPlayer], sizeof(int) * MAX_DECK);
	G.deckCount[currentPlayer] = pre.deckCount[currentPlayer];

	// Reverse changes from discardCard()
	memcpy(G.playedCards, pre.playedCards, sizeof(int) * MAX_DECK);
	G.playedCardCount = pre.playedCardCount;
	memcpy(G.discard[currentPlayer], pre.discard[currentPlayer], sizeof(int) * MAX_DECK);
	G.discardCount[currentPlayer] = pre.discardCount[currentPlayer];

	// Check if changes made to other players
	assertCheck(memcmp(&pre, &G, sizeof(struct gameState)), 0, &numError, "No modifications to other gameState variables", "Other gameState variables were modified");

	// -----------------Test Case 4-----------------------//
	printf("\n----------Test Case 4 - Treasure cards(1) in discard----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// RESET game
	bonus = NULL;
	currentPlayer = whoseTurn(&G);
	handPos = G.handCount[currentPlayer];

	// Add adventurer to player hand
	G.hand[currentPlayer][handPos] = adventurer;
	G.handCount[currentPlayer]++;
	G.supplyCount[adventurer]--;

	// 1 Treasure card in discard
	for (int i = 0; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = estate;
	}
	G.discard[currentPlayer][0] = copper;
	G.discardCount[currentPlayer] = 1;

	pre = G;									// save game state before cardEffect() calls adventurer card
	assert(cardEffect(adventurer, copper, silver, gold, &G, handPos, bonus) == 0);		// play adventurer card

	// Check 1 card added to hand from deck
	assertCheck(pre.handCount[currentPlayer], G.handCount[currentPlayer], &numError, "Hand count remained the same.", "Incorrect hand count.");
	assertCheck(pre.deckCount[currentPlayer] - 1, G.deckCount[currentPlayer], &numError, "Deck count remained the same.", "Incorrect deck count.");

	// Check if that card is a treasure card
	cardInHand = G.hand[currentPlayer][G.handCount[currentPlayer] - 1];
	switch (cardInHand) {
		case copper:
		case silver:
		case gold:
			break;
		default:
			numError++;
			printf("ERROR: Not a treasure card.\n");
			break;
	}
	// Check card should come from his own pile.
	// No state change should occur for other players.
	// No state change should occur to the victory card piles and kingdom card piles

	// Reverse changes from adventurer card
	memcpy(G.hand[currentPlayer], pre.hand[currentPlayer], sizeof(int) * MAX_HAND);
	G.handCount[currentPlayer] = pre.handCount[currentPlayer];
	memcpy(G.deck[currentPlayer], pre.deck[currentPlayer], sizeof(int) * MAX_DECK);
	G.deckCount[currentPlayer] = pre.deckCount[currentPlayer];

	// Reverse changes from discardCard()
	memcpy(G.playedCards, pre.playedCards, sizeof(int) * MAX_DECK);
	G.playedCardCount = pre.playedCardCount;
	memcpy(G.discard[currentPlayer], pre.discard[currentPlayer], sizeof(int) * MAX_DECK);
	G.discardCount[currentPlayer] = pre.discardCount[currentPlayer];

	// Check if changes made to other players
	assertCheck(memcmp(&pre, &G, sizeof(struct gameState)), 0, &numError, "No modifications to other gameState variables", "Other gameState variables were modified");

	// Print test results
	if (numError > 0) {
		printf("\n!!!!!----------TEST FAILED: Testing Adventurer----------!!!!!\n");
		printf("Number of errors: %d\n\n", numError);
	}
	else {
		printf("\n----------SUCCESS: Testing complete Adventurer----------\n\n");
	}

	return 0;
}