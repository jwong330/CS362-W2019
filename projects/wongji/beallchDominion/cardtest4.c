#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NUMCARDS 10

/* Tests Great Hall card */
//Description:Adds one card to player's hand from own deck.
//		Increases number of actions by 1 for the player.

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
	int * bonus = NULL;
	int currentPlayer;
	int handPos;
	int numError = 0;

	printf("----------Testing Great Hall Card----------\n");

	// -----------------Test Case 1-----------------------//
	printf("----------Test Case 1 - +1 card from deck----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Initialize game
	currentPlayer = whoseTurn(&G);
	handPos = G.handCount[currentPlayer];
	
	// Add great hall to player hand
	G.hand[currentPlayer][handPos] = great_hall;
	G.handCount[currentPlayer]++;
	G.supplyCount[great_hall]--;
	
	pre = G;									// save game state before cardEffect() calls great hall card
	assert(cardEffect(great_hall, copper, silver, gold, &G, handPos, bonus) == 0);		// play great hall card

	// Check 1 cards added to hand from deck
	assertCheck(pre.handCount[currentPlayer], G.handCount[currentPlayer], &numError, "Hand count stayed the same.", "Incorrect hand count");
	if (memcmp(pre.hand[currentPlayer], G.hand[currentPlayer], sizeof(int) * MAX_HAND) == 0) {
		numError++;
		printf("ERROR: Hand stayed the same.\n");
	}
	assertCheck(pre.deckCount[currentPlayer] - 1, G.deckCount[currentPlayer], &numError, "Deck count decreased by 1.", "Incorrect deck count.");
	assertCheck(pre.numActions + 1, G.numActions, &numError, "Number of actions increased by 1.", "Incorrect number of actions.");

	// Check 1 card should come from his own pile.
	// No state change should occur for other players.
	// No state change should occur to the victory card piles and kingdom card piles
	
	// Reverse changes from great hall card
	memcpy(G.hand[currentPlayer], pre.hand[currentPlayer], sizeof(int) * MAX_HAND);
	G.handCount[currentPlayer] = pre.handCount[currentPlayer];
	memcpy(G.deck[currentPlayer], pre.deck[currentPlayer], sizeof(int) * MAX_DECK);
	G.deckCount[currentPlayer] = pre.deckCount[currentPlayer];
	G.numActions = pre.numActions;

	// Reverse changes from discardCard()
	memcpy(G.playedCards, pre.playedCards, sizeof(int) * MAX_DECK);
	G.playedCardCount = pre.playedCardCount;
	memcpy(G.discard[currentPlayer], pre.discard[currentPlayer], sizeof(int) * MAX_DECK);
	G.discardCount[currentPlayer] = pre.discardCount[currentPlayer];

	// Check if changes made to other players
	assertCheck(memcmp(&pre, &G, sizeof(struct gameState)), 0, &numError, "No modifications to other gameState variables", "Other gameState variables were modified");

	// Print test results
	if (numError > 0) {
		printf("\n!!!!!----------TEST FAILED: Testing Great Hall ----------!!!!!\n");
		printf("Number of errors: %d\n\n", numError);
	}
	else {
		printf("\n ----------SUCCESS: Testing complete Great Hall ----------\n\n");
	}

	return 0;
}