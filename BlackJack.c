#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>

typedef struct Card {
    wchar_t suit;    
    char rank[2];
    int value;    
    struct Card* next; 
} Card;


typedef struct {
    Card* head;
    int count;
} Deck;


Card* createCard(wchar_t suit, const char* rank, int value) {
    Card* newCard = (Card*)malloc(sizeof(Card));
    newCard->suit = suit;
    int i = 0;
    while (rank[i] != '\0' && i < 2) {
        newCard->rank[i] = rank[i];
        i++;
    }
    newCard->rank[i] = '\0';
    newCard->value = value;
    newCard->next = NULL;
    return newCard;
}


Deck* initDeck() {
    Deck* deck = (Deck*)malloc(sizeof(Deck));
    deck->head = NULL;
    deck->count = 0;

    wchar_t suits[] = {L'♠', L'♥', L'♦', L'♣'};
    char* ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int values[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};


    for (int s = 0; s < 4; s++) {
        for (int r = 0; r < 13; r++) {
            Card* newCard = createCard(suits[s], ranks[r], values[r]);
            newCard->next = deck->head;
            deck->head = newCard;
            deck->count++;
        }
    }
    return deck;
}


void shuffleDeck(Deck* deck) {
    if (deck->count <= 1) return;

    Card** cards = (Card**)malloc(deck->count * sizeof(Card*));
    Card* current = deck->head;
    int i = 0;
    

    while (current != NULL) {
        cards[i++] = current;
        current = current->next;
    }


    for (i = deck->count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card* temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }


    for (i = 0; i < deck->count - 1; i++) {
        cards[i]->next = cards[i + 1];
    }
    cards[deck->count - 1]->next = NULL;
    deck->head = cards[0];

    free(cards);
}


Card* drawCard(Deck* deck) {
    if (deck->head == NULL) return NULL;
    
    Card* card = deck->head;
    deck->head = deck->head->next;
    deck->count--;
    card->next = NULL;
    return card;
}


void printCard(Card* card) {
    printf("┌─────┐\n");
    printf("│%s%-2s  │\n", card->rank, " ");
    printf("│  %lc  │\n", card->suit);
    printf("│  %-2s%s│\n", card->rank, " ");
    printf("└─────┘\n");
}

int main() {
    setlocale(LC_ALL, "");  
    srand(time(NULL));      

    Deck* deck = initDeck();
    shuffleDeck(deck);

    Card* playerHand = NULL;
    Card* dealerHand = NULL;
    int playerScore = 0;
    int dealerScore = 0;
    char choice;

    for (int i = 0; i < 2; i++) {
        Card* playerCard = drawCard(deck);
        playerCard->next = playerHand;
        playerHand = playerCard;
        playerScore += playerCard->value;

        Card* dealerCard = drawCard(deck);
        dealerCard->next = dealerHand;
        dealerHand = dealerCard;
        dealerScore += dealerCard->value;
    }

    printf("\nВаши карты:\n");
    Card* current = playerHand;
    while (current != NULL) {
        printCard(current);
        current = current->next;
    }
    printf("Ваш счёт: %d\n", playerScore);

    printf("\nКарта дилера:\n");
    printCard(dealerHand);
    printf("Остальные карты дилера скрыты\n");

    while (playerScore < 21) {
        printf("\nВзять карту? (y/n): ");
        scanf(" %c", &choice);
        
        if (choice == 'n' || choice == 'N') break;
        
        if (choice == 'y' || choice == 'Y') {
            Card* newCard = drawCard(deck);
            newCard->next = playerHand;
            playerHand = newCard;
            playerScore += newCard->value;

            printf("\nВаш�� карты:\n");
            current = playerHand;
            while (current != NULL) {
                printCard(current);
                current = current->next;
            }
            printf("Ваш счёт: %d\n", playerScore);
        }
    }

    printf("\nКарты дилера:\n");
    current = dealerHand;
    while (current != NULL) {
        printCard(current);
        current = current->next;
    }

    while (dealerScore < 17 && playerScore <= 21) {
        Card* newCard = drawCard(deck);
        newCard->next = dealerHand;
        dealerHand = newCard;
        dealerScore += newCard->value;
        
        printf("\nДилер берёт карту:\n");
        printCard(newCard);
    }
    printf("Счёт дилера: %d\n", dealerScore);

    printf("\nИтоговый счёт:\n");
    printf("Вы: %d\n", playerScore);
    printf("Дилер: %d\n", dealerScore);

    if (playerScore > 21) {
        printf("Перебор! Вы проиграли.\n");
    } else if (dealerScore > 21) {
        printf("Дилер перебрал! Вы выиграли!\n");
    } else if (playerScore > dealerScore) {
        printf("Поздравляем! Вы выиграли!\n");
    } else if (playerScore < dealerScore) {
        printf("Вы проиграли.\n");
    } else {
        printf("Ничья!\n");
    }

    while (playerHand != NULL) {
        Card* temp = playerHand;
        playerHand = playerHand->next;
        free(temp);
    }
    while (dealerHand != NULL) {
        Card* temp = dealerHand;
        dealerHand = dealerHand->next;
        free(temp);
    }
    while (deck->head != NULL) {
        Card* temp = deck->head;
        deck->head = deck->head->next;
        free(temp);
    }
    free(deck);

    return 0;
}