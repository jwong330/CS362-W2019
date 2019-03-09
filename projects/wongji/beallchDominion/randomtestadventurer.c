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
	// Initialize game state
	srand(time(NULL));
	int seed = 100;
	int k[NUMCARDS] = { adventurer, council_room, feast, gardens, mine, remodel,
					smithy, village, baron, great_hall };
	struct gameState pre, G;
	int * bonus;
	int currentPlayer;
	int handPos;
	int numError = 0;

	printf("----------Testing Adventurer Card----------\n");

	// -----------------Test Case-----------------------//
	for (int i = 0; i < 50; i++) {
		printf("----------Test Case %d - Treasure cards(0 - 2+)----------\n", i + 1);
		assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Initialize game
		bonus = NULL;
		currentPlayer = whoseTurn(&G);
		handPos = rand() % G.handCount[currentPlayer];			// randomize card position

		// Add adventurer to player hand
		G.hand[currentPlayer][handPos] = adventurer;
		G.supplyCount[adventurer]--;

		// Randomize deck

		// set first two cards to non-treasure, set rest to a treasure card
		G.deck[currentPlayer][0] = G.deck[currentPlayer][1] = estate;
		for (int i = 2; i < MAX_DECK; i++) {
			G.deck[currentPlayer][i] = copper + rand() % 3;
		}
		// Randomize deck count 2 - 5
		// 0 treasure, 1 treasure, 2 treasure, 2+ treasure
		G.deckCount[currentPlayer] = rand() % 4 + 2;

		// Set discard pile to treasures
		for (int i = 0; i < MAX_DECK; i++) {
			G.discard[currentPlayer][i] = copper + rand() % 3;
		}

		// Randomize dicard count 0 - 3
		// Empty, 1 treasure, 2 treasure, 2+ treasure
		G.discardCount[currentPlayer] = rand() % 4;


		int totalTreasureCount = G.deckCount[currentPlayer] - 2 + G.discardCount[currentPlayer];
		printf("Treasure card in deck: %d	Treasure card in discard: %d\n", G.deckCount[currentPlayer] - 2, G.discardCount[currentPlayer]);

		pre = G;									// save game state before randomizing game state and cardEffect() calls adventurer card

		assert(cardEffect(adventurer, copper, silver, gold, &G, handPos, bonus) == 0);	// play adventurer card

		// Check numTreasure cards added to hand from deck
		// deckcount
		// discardcount
		// 0 0
		// 1 1
		// 2 2
		switch (totalTreasureCount) {
		case 0:
			printf("Hand count expected : %d Result : %d\n", pre.handCount[currentPlayer] - 1, G.handCount[currentPlayer]);
			assertCheck(pre.handCount[currentPlayer] - 1, G.handCount[currentPlayer], &numError, "Hand count decreased by 1.", "Hand count did NOT decrease by 1.");
			break;
		case 1:
			printf("Hand count expected : %d Result : %d\n", pre.handCount[currentPlayer], G.handCount[currentPlayer]);
			assertCheck(pre.handCount[currentPlayer], G.handCount[currentPlayer], &numError, "Hand count stayed the same.", "Hand count did NOT stay the same.");
			break;
		default:
			printf("Hand count expected : %d Result : %d\n", pre.handCount[currentPlayer] + 1, G.handCount[currentPlayer]);
			assertCheck(pre.handCount[currentPlayer] + 1, G.handCount[currentPlayer], &numError, "Hand count increased by 1.", "Hand count did NOT increase by 1.");
			break;
		}

		// Check if those cards are treasure cards
		for (int location = 0; location < 2 && location < totalTreasureCount; location++) {
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

		// Check treasure cards should come from his own pile.
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
	}

	// Print test results
	if (numError > 0) {
		printf("\n!!!!!----------TEST FAILED: Testing Adventurer----------!!!!!\n");
		printf("Number of errors: %d\n\n", numError);
	}
	else {
		printf("\n----------SUCCESS: Testing complete Adventurer----------\n\n");
	}

	//system("pause");

	return 0;
}
