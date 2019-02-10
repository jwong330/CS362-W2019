#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NUMCARDS 10

/* Tests gainCard() */
//Description:Adds a specific card to a players hand, deck, or trash.
//		Checks for enough supply of the card then puts the card in the directed location.

// checkCardAdded()
// Check if new card added to end of card pile
// Input: player, newCard, cardLocation(card pile), gamestate
int checkCardAdded(int player, int newCard, int cardLocation, struct gameState * state) {
	int found = 0;
	switch (cardLocation) {
		case 0: //discard
				if (newCard == state->discard[player][state->discardCount[player] - 1]) {
					found = 1;
				}
				break;
		case 1: //deck
				if (newCard == state->deck[player][state->deckCount[player] - 1]) {
					found = 1;
				}
				break;
		case 2: //hand
				if (newCard == state->hand[player][state->handCount[player] - 1]) {
					found = 1;
				}
				break;
		default:
				printf("Invalid cardLocation input.\n");
				break;
	}

	return found;
}

// checkSupplyCount()
// Compares if supply count changed by correct amount
// Input: card, pre-gamestate, and post-gamestate
void checkSupplyCount(int card, struct gameState * pre, struct gameState * post) {
	assert(pre->supplyCount[card] == post->supplyCount[card] + 1);
	pre->supplyCount[card]--;
	assert(memcmp(pre->supplyCount, post->supplyCount, sizeof(post->supplyCount)) == 0);
	// restore pre gameState
	pre->supplyCount[card]++;
}

// cmpGameState()
// Compares if only change to cardLocation pile is made to gameState
// Input: player, card, cardLocation(card pile), pre-gamestate, and post-gamestate
int cmpGameState(int player, int card, int cardLocation, struct gameState * pre, struct gameState * post) {
	switch (cardLocation) {
	case 0: // discard
			pre->discard[player][pre->discardCount[player]] = card;
			pre->discardCount[player]++;
			pre->supplyCount[card]--;
			assert(memcmp(pre, post, sizeof(struct gameState)) == 0);

			// restore pre gameState
			pre->supplyCount[card]++;
			pre->discardCount[player]--;
			pre->discard[player][pre->discardCount[player]] = -1;

			break;
	case 1: // deck
			pre->deck[player][pre->deckCount[player]] = card;
			pre->deckCount[player]++;
			pre->supplyCount[card]--;
			assert(memcmp(pre, post, sizeof(struct gameState)) == 0);

			// restore pre gameState
			pre->supplyCount[card]++;
			pre->deckCount[player]--;
			pre->deck[player][pre->deckCount[player]] = -1;

			break;
	case 2: // hand
			pre->hand[player][pre->handCount[player]] = card;
			pre->handCount[player]++;
			pre->supplyCount[card]--;
			assert(memcmp(pre, post, sizeof(struct gameState)) == 0);

			// restore pre gameState
			pre->supplyCount[card]++;
			pre->handCount[player]--;
			pre->hand[player][pre->handCount[player]] = -1;

			break;
	default:
			printf("Invalid cardLocation input.\n");
			break;
	}

	return 0;
}

int main() {
	int seed = 100;
	int k[NUMCARDS] = { adventurer, council_room, feast, gardens, mine, remodel,
					smithy, village, baron, great_hall };
	struct gameState pre, G;
	printf("----------Testing gainCard()----------\n");

	// Black Box Test
	// -----------------Test Case 1-----------------------//
	printf("----------Test Case 1 - add card to different piles----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Initialize game
	// cardlocation = 0 discard
	// cardlocation = 1 deck
	// cardlocation = 2 hand
	for (int cardLocation = 0; cardLocation < 3; cardLocation++) {
		for (int player = 0; player < MAX_PLAYERS; player++) {
			for (int card = 0; card < NUMCARDS; card++) {
				pre = G;									// save game state before gainCard().
				assert(gainCard(k[card], &G, cardLocation, player) == 0);
				assert(checkCardAdded(player, k[card], cardLocation, &G) == 1);	// check if card is in cardLocation
				cmpGameState(player, k[card], cardLocation, &pre, &G);			// check if card is in hand
				checkSupplyCount(k[card], &pre, &G);					// check if supply changed
				
			}
		}
		assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Reset game
	}

	// -----------------Test Case 2-----------------------//
	printf("----------Test Case 2 - add card from empty supply pile----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Initialize game
	// set all cards in supply to 0
	for (int supplyCard = 0; supplyCard < sizeof(G.supplyCount) / sizeof(int); supplyCard++) {
		G.supplyCount[supplyCard] = 0;							
	}
	// try to add card from empty supply
	for (int cardLocation = 0; cardLocation < 3; cardLocation++) {
		for (int player = 0; player < MAX_PLAYERS; player++) {
			for (int card = 0; card < NUMCARDS; card++) {
				pre = G;									// save game state before gainCard()
				assert(gainCard(k[card], &G, cardLocation, player) == -1);		// check if unable to add card
				assert(memcmp(&pre, &G, sizeof(struct gameState)) == 0);		// check if game state changed
			}
		}
	}

	// -----------------Test Case 3-----------------------//
	printf("----------Test Case 3 - add card that is not in play----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Initialize game
	// all cards in supply are not used
	for (int supplyCard = 0; supplyCard < sizeof(G.supplyCount) / sizeof(int); supplyCard++) {
		G.supplyCount[supplyCard] = -1;
	}
	// try to add card not in use
	for (int cardLocation = 0; cardLocation < 3; cardLocation++) {
		for (int player = 0; player < MAX_PLAYERS; player++) {
			for (int card = 0; card < NUMCARDS; card++) {
				pre = G;									// save game state before gainCard()
				assert(gainCard(k[card], &G, cardLocation, player) == -1);
				assert(memcmp(&pre, &G, sizeof(struct gameState)) == 0);		// check if game state changed
			}
		}
	}

	printf("\n----------SUCCESS: Testing complete gainCard()----------\n\n");

	return 0;
}