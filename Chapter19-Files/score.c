#include <cx16.h>
#include <cbm.h>
#include <stdio.h>
#include <string.h>

typedef struct Score {
    unsigned char name[4];
    unsigned short score;
} Score;

// We use a fixed size array here
// We could also be more memory efficient and:
// 1. Read the file into a temp buffer
// 2. Use 'malloc' to allocate exactly how much we need
// 3. Copy the bytes from the temp buffer
// BUT...this is easier for an example.
Score scoreData[100];

unsigned char scoreDataRecords;

void addHighScore(Score *newScore) {
    // Copy the bytes from the score struct into our score array
    memcpy(&scoreData[scoreDataRecords], newScore, sizeof(Score));

    scoreDataRecords++;

    // Save the new high.bin file
    // NOTE: the "@:" prefix allows us to overwrite a file
    cbm_k_setnam("@:high.bin");

    // SAVE adds the 2 byte header and we can't stop it
    // NOTE: CX16 also offers "BSAVE" which is the same as SAVE but no 2 byte header
    // cc65 doesn't have a C function for this though
    cbm_k_setlfs(0, 8, 2);

    cbm_k_save((unsigned short)scoreData, ((unsigned short)scoreData) + (sizeof(Score) * scoreDataRecords));

    printf("Added Score: %s - %u\n\n", newScore->name, newScore->score);
}

void loadHighScores() {
    unsigned short finalMemAddr, bytesRead;

    cbm_k_setnam("high.bin");
    cbm_k_setlfs(0, 8, 0); // File has the 2 byte header but skip it

    // load returns the last memory address written to
    finalMemAddr = cbm_k_load(0, (unsigned short)scoreData);

    // Calculate the number of bytes read as the difference between
    // the starting and ending addresses
    bytesRead = finalMemAddr - (unsigned short)scoreData;

    // Calculate the number of "Score" records we read as:
    // The total bytes read / the sizeof a Score struct "sizeof(Score)" 
    scoreDataRecords = bytesRead / sizeof(Score);

    printf("Bytes read from high.bin: %u\n", bytesRead);
    printf("Number of Score Records: %u\n", scoreDataRecords);
}

void printHighScores() {
    unsigned char i;
    
    printf("\n\nHIGH SCORE LIST\n--------------\n");

    for (i=0; i<scoreDataRecords; i++) {
        printf("%s - %u\n", scoreData[i].name, scoreData[i].score);
    }

    printf("\n");
}

void main() {
    Score newScore = { "FOO", 5684 };

    loadHighScores();
    printHighScores();

    addHighScore(&newScore);

    loadHighScores();
    printHighScores();

}