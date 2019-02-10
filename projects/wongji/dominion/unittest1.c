#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NUMCARDS 10

/* Tests fullDeckCount() */
// Description:Returns the count of a specific card in a specific player’s discard, deck and hand.

int main() {
	int seed = 100;
	int numPlayer = 2;
	int k[NUMCARDS] = { adventurer, council_room, feast, gardens, mine, remodel,
					smithy, village, baron, great_hall };
	int cardCount = 5;
	struct gameState pre, G;

	printf("----------Testing fullDeckCount()----------\n");
	// Black Box Test

	// -----------------Test Case 1-----------------------//
	printf("----------Test Case 1 - %d card in deck, 0 in others----------\n", cardCount);
	assert(initializeGame(numPlayer, k, seed, &G) == 0);		// Initialize game
	for (int player = 0; player < numPlayer; player++) {
		for (int card = 0; card < NUMCARDS; card++) {
				
			// make CARDCOUNT copies of card in deck
			G.deckCount[player] = cardCount;
			for (int i = 0; i < cardCount; i++) {
				G.deck[player][i] = k[card];
			}
			pre = G;											// save game state before fullDeckCount()
			int cardInDeck = fullDeckCount(player, k[card], &G);
			assert(cardCount == cardInDeck);
			assert(memcmp(&pre, &G, sizeof(struct gameState)) == 0); // check if game state was changed
		}
	}

	// -----------------Test Case 2-----------------------//
	printf("----------Test Case 2 - %d card in hand, 0 in others----------\n", cardCount);
	assert(initializeGame(numPlayer, k, seed, &G) == 0);		// Reset game
	for (int player = 0; player < numPlayer; player++) {
		for (int card = 0; card < NUMCARDS; card++) {

			// make CARDCOUNT copies of card in deck
			G.handCount[player] = cardCount;
			for (int i = 0; i < cardCount; i++) {
				G.hand[player][i] = k[card];
			}
			pre = G;											// save game state before fullDeckCount()
			int cardInHand = fullDeckCount(player, k[card], &G);
			assert(cardCount == cardInHand);
			assert(memcmp(&pre, &G, sizeof(struct gameState)) == 0); // check if game state was changed
		}
	}

	// -----------------Test Case 3-----------------------//
	printf("----------Test Case 3 - %d card in discard, 0 in others----------\n", cardCount);
	assert(initializeGame(numPlayer, k, seed, &G) == 0);		// Reset game
	for (int player = 0; player < numPlayer; player++) {
		for (int card = 0; card < NUMCARDS; card++) {

			// make CARDCOUNT copies of card in deck
			G.discardCount[player] = cardCount;
			for (int i = 0; i < cardCount; i++) {
				G.discard[player][i] = k[card];
			}
			pre = G;											// save game state before fullDeckCount()
			int cardInDiscard = fullDeckCount(player, k[card], &G);
			assert(cardCount == cardInDiscard);
			assert(memcmp(&pre, &G, sizeof(struct gameState)) == 0); // check if game state was changed
		}
	}

	// -----------------Test Case 4-----------------------//
	printf("----------Test Case 4 - different card counts in all piles----------\n");
	assert(initializeGame(numPlayer, k, seed, &G) == 0);		// Reset game
	for (int player = 0; player < numPlayer; player++) {
		for (int card = 0; card < NUMCARDS; card++) {
			// Fill player's hand, deck, discard with copies of one kingdom card
			for (int i = 0; i < cardCount; i++) {
				G.deck[player][i] = k[card];
				G.hand[player][i] = k[card];
				G.discard[player][i] = k[card];
			}
			
			// set different counts for each pile
			G.deckCount[player] = cardCount - 2;
			G.handCount[player] = cardCount - 1;
			G.discardCount[player] = cardCount;
			
			// count total copies of card in all piles
			pre = G;											// save game state before fullDeckCount()
			int cardInAllPiles = fullDeckCount(player, k[card], &G);
			int totalCount = G.deckCount[player] + G.handCount[player] + G.discardCount[player];
			assert(cardInAllPiles == totalCount);
			assert(memcmp(&pre, &G, sizeof(struct gameState)) == 0); // check if game state was changed
		}
	}

	// -----------------Test Case 5-----------------------//
	printf("----------Test Case 5 - 0 card in all piles----------\n");
	assert(initializeGame(numPlayer, k, seed, &G) == 0);		// Reset game
	for (int player = 0; player < numPlayer; player++) {
		for (int card = 0; card < NUMCARDS; card++) {
			// no card added to any pile
			pre = G;											// save game state before fullDeckCount()
			int cardInAllPiles = fullDeckCount(player, k[card], &G);
			assert(cardInAllPiles == 0);
			assert(memcmp(&pre, &G, sizeof(struct gameState)) == 0); // check if game state was changed
		}
	}

	// WB
	// -----------------Test Case 6-----------------------//
	printf("----------Test Case 6 - max card count in all piles----------\n");
	assert(initializeGame(numPlayer, k, seed, &G) == 0);		// Reset game
	for (int player = 0; player < MAX_PLAYERS; player++) {
		for (int card = 0; card < NUMCARDS; card++) {
			// Fill player's hand, deck, discard with copies of one kingdom card
			for (int i = 0; i < MAX_DECK; i++) {
				G.deck[player][i] = k[card];
				G.discard[player][i] = k[card];
			}
			for (int i = 0; i < MAX_HAND; i++) {
				G.hand[player][i] = k[card];
			}

			// set different counts for each pile
			G.deckCount[player] = MAX_DECK;
			G.handCount[player] = MAX_HAND;
			G.discardCount[player] = MAX_DECK;

			// count total copies of card in all piles
			pre = G;											// save game state before fullDeckCount()
			int cardInAllPiles = fullDeckCount(player, k[card], &G);
			int totalCount = MAX_DECK + MAX_HAND + MAX_DECK;
			assert(cardInAllPiles == totalCount);
			assert(memcmp(&pre, &G, sizeof(struct gameState)) == 0); // check if game state was changed
		}
	}

	 // -----------------Test Case 7-----------------------//
	printf("----------Test Case 7 - negative card count in all piles----------\n");
	assert(initializeGame(numPlayer, k, seed, &G) == 0);		// Reset game
	for (int player = 0; player < MAX_PLAYERS; player++) {
		for (int card = 0; card < NUMCARDS; card++) {
			// Fill player's hand, deck, discard with copies of one kingdom card
			for (int i = 0; i < MAX_DECK; i++) {
				G.deck[player][i] = k[card];
				G.discard[player][i] = k[card];
			}
			for (int i = 0; i < MAX_HAND; i++) {
				G.hand[player][i] = k[card];
			}

			// set negative counts for each pile
			G.deckCount[player] = -1;
			G.handCount[player] = -1;
			G.discardCount[player] = -1;

			// count total copies of card in all piles
			pre = G;											// save game state before fullDeckCount()
			int cardInAllPiles = fullDeckCount(player, k[card], &G);
			assert(cardInAllPiles == 0);
			assert(memcmp(&pre, &G, sizeof(struct gameState)) == 0); // check if game state was changed
		}
	}
	
	printf("\n----------SUCCESS: Testing complete fullDeckCount()----------\n\n");

	return 0;
}