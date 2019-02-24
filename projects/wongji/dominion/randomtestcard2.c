#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define NUMCARDS 10

/* Tests Village card */
//Description:Adds one card to a players hand.
//		Increments number of actions by 2 for player.


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
	// Initialize game state
	srand(time(NULL));
	int seed = 100;
	int k[NUMCARDS] = { adventurer, council_room, feast, gardens, mine, remodel,
					smithy, village, baron, great_hall };
	struct gameState pre, G;
	int * bonus = NULL;
	int currentPlayer;
	int handPos;
	int numError = 0;

	printf("----------Testing Village Card----------\n");

	// -----------------Test Case-----------------------//
	for (int i = 0; i < 10; i++) {
		printf("----------Test Case %d - Village card(2 - 5 deck count)----------\n", i + 1);
		assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Initialize game
		currentPlayer = whoseTurn(&G);
		handPos = rand() % G.handCount[currentPlayer];		// randomize card position

		// Add village to player hand
		G.hand[currentPlayer][handPos] = village;
		G.supplyCount[village]--;

		// Randomization
		G.deckCount[currentPlayer] = rand() % 4 + 2;		// random deck count range is : 1 to 5

		pre = G;									// save game state before cardEffect() calls village card
		assert(cardEffect(village, copper, silver, gold, &G, handPos, bonus) == 0);		// play village card

		// Check 1 card added to hand from deck
		assertCheck(pre.handCount[currentPlayer], G.handCount[currentPlayer], &numError, "Hand count remained the same.", "Hand count did NOT remain the same.");
		assertCheck(pre.deckCount[currentPlayer] - 1, G.deckCount[currentPlayer], &numError, "Deck count decreased by 1.", "Deck count did NOT decrease by 1.");

		// Villager was in the last position of the player's hand. Check that now it is a different card taken from the deck
		if (G.hand[currentPlayer][handPos] == village) {
			printf("ERROR: A new card was not added to player's hand.\n");
		}

		// Check 2 actions were added to player
		assertCheck(pre.numActions + 2, G.numActions, &numError, "Number of actions increased by 2.", "Number of actions did NOT increase by 2.");

		// Check 1 cards should come from his own pile.
		// No state change should occur for other players.
		// No state change should occur to the victory card piles and kingdom card piles

		// Reverse changes from village card
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
	}

	// Print test results
	if (numError > 0) {
		printf("\n!!!!!----------TEST FAILED: Testing Village----------!!!!!\n");
		printf("Number of errors: %d\n\n", numError);
	}
	else {
		printf("\n----------SUCCESS: Testing complete Village----------\n\n");
	}

	//system("pause");

	return 0;
}