void Node::checkChild(int *indexToAdd, int *positionInWord, int *remainingCount, int wordLength, char *word, char matchingWords[100][MAX_LENGTH + 1], int *count)
{       
        if (getLetter() != word[positionInWord])
                remainingCount--;
        
        if (remainingCount == 0)
                return;
        
        if (positionInWord == wordLength - 1)
        {       
                char wordToAdd[34];
                getWord(wordToAdd, wordLength);
                strcpy(matchingWords[indexToAdd], wordToAdd);
                *count++;
                indexToAdd++;
                return;
        }
        for (int i = 0; i < childrenCount; i++)
        {       
                children[i]->checkChild(indexToAdd, positionInWord++; remainingCount, wordLength, word, matchingWords, count);
        }
}