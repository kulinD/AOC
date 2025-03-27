#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// hand ranking low to high
enum HandType {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
};

// Represents a poker hand with card value, bid, evaluated hand
typedef struct {
    enum HandType handType;
    int32_t cards_values[5];
    int32_t bid_amount;
} Hand;

Hand hands[1000] = {0};

// convert card char to numeric value
int32_t card_to_value(char c) {
    char* card_order = "23456789TJQKA";
    int32_t idx = strchr(card_order, c) - card_order;
    return idx;
}

// classify hand without jokers
enum HandType classify_hand(Hand hand) {
    int32_t counts[13] = {0};

    for (int32_t idx = 0; idx < 5; idx++) {
        counts[hand.cards_values[idx]]++;
    }

    // count occurences of values
    int32_t match_counts[6] = {0};
    for (int32_t match_count = 1; match_count <= 5; match_count++) {
        for (int32_t idx = 0; idx < 13; idx++) {
            if (counts[idx] == match_count)
                match_counts[match_count]++;
        }
    }

    // determine hand type
    if (match_counts[5] == 1) return FIVE_OF_A_KIND;
    if (match_counts[4] == 1) return FOUR_OF_A_KIND;
    if (match_counts[3] == 1 && match_counts[2] == 1) return FULL_HOUSE;
    if (match_counts[3] == 1) return THREE_OF_A_KIND;
    if (match_counts[2] == 2) return TWO_PAIR;
    if (match_counts[2] == 1) return ONE_PAIR;
    return HIGH_CARD;
}

// sorting hands by strength
int32_t hand_sort(const void *_left, const void *_right) {
    const Hand *left = (const Hand *)_left;
    const Hand *right = (const Hand *)_right;

    if (left->handType != right->handType) {
        return left->handType - right->handType;
    }

    // for ties
    for (int32_t card = 0; card < 5; card++) {
        if (left->cards_values[card] != right->cards_values[card]) {
            return left->cards_values[card] - right->cards_values[card];
        }
    }
    return 0;
}

// replacing cards for jokers to give best possible hand
enum HandType best_joker_hand(Hand hand, int32_t card_idx) {
    if (card_idx == 5) {
        return classify_hand(hand);
    }

    enum HandType best_hand_type = HIGH_CARD;
    int32_t joker_value = card_to_value('J');

    // replace joker with possible card values
    if (hand.cards_values[card_idx] == joker_value) {
        for (int32_t alt_value = 0; alt_value < 13; alt_value++) {
            Hand alt_hand = hand;
            alt_hand.cards_values[card_idx] = alt_value;
            enum HandType alt_hand_type = best_joker_hand(alt_hand, card_idx + 1);
            if (alt_hand_type > best_hand_type) {
                best_hand_type = alt_hand_type;
            }
        }
    } else {
        // skip non-joker cards
        best_hand_type = best_joker_hand(hand, card_idx + 1);
    }

    return best_hand_type;
}

// handling hand with joker
enum HandType classify_joker_hand(Hand hand) {
    int32_t joker_value = card_to_value('J');
    int32_t num_jokers = 0;

    for (int32_t idx = 0; idx < 5; idx++) {
        if (hand.cards_values[idx] == joker_value)
            num_jokers++;
    }

    if (num_jokers >= 4) {
        return FIVE_OF_A_KIND;
    }

    return best_joker_hand(hand, 0);
}

int32_t count_winnings(Hand *hands, int32_t num_hands) {
    qsort(hands, num_hands, sizeof(Hand), hand_sort);
    int32_t winnings = 0;

    // rank * bid
    for (int32_t idx = 0; idx < num_hands; idx++) {
        winnings += (idx + 1) * hands[idx].bid_amount;
    }
    return winnings;
}

int32_t main(void) {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        printf("Failed to open input file");
        return 1;
    }

    char line[256];
    int32_t num_hands = 0;

    // parse cards to numeric values
    while (fgets(line, sizeof(line), file) != NULL) {
        for (int32_t card_no = 0; card_no < 5; card_no++) {
            hands[num_hands].cards_values[card_no] = card_to_value(line[card_no]);
        }
        // parse bid starting at line 6
        hands[num_hands].bid_amount = strtol(&line[6], NULL, 10);
        hands[num_hands].handType = classify_hand(hands[num_hands]);
        num_hands++;
    }
    fclose(file);

    int32_t winnings = count_winnings(hands, num_hands);
    printf("Part 1 Winnings: %d \n", winnings);

    // Adjust hand for part2 jokers
    for( int idx =0; idx<num_hands; idx++ ) {
        hands[ idx ].handType = classify_joker_hand( hands[ idx ] );
        // remap the values to account for Joker's new drop in value
        int new_value[ 13 ] = { 1, 2,3,4,5,6,7,8,9, 0, 10, 11 ,12 };
        for( int card_no=0; card_no<5; card_no++ ) {
            hands[idx].cards_values[card_no] = new_value[ hands[idx].cards_values[card_no] ];
        }
    }

    winnings = count_winnings(hands, num_hands);
    printf("Part 2 Winnings: %d \n", winnings);

    return 0;
}
