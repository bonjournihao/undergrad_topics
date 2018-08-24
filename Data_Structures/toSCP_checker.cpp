Node *ptr = root;
int error = 0;

for (unsigned int m = 0; m < strlen(word); m++)
{
	int error = 0;
	if (ptr->hasChild(word[m]))
		ptr = ptr->hasChild(word[m]);
	else
		error = -1; break;
}


if (error == 0)
{
	char wordToCopy[34];
	*count = 1;
	ptr->getWord(wordToCopy, strlen(word));
	strcpy(matchingWords[j], wordToCopy);
	return;
}



//if no exact word, allow (one error at ith position...
Node *errorPtr[strlen(word)];
for (unsigned int i = 0; i < strlen(word); i++)
{
	Node *ptr2 = root;
	int error = 0;
	//check front 1..i-1 
	int m = 0;
	unsigned int j;
	for (j = 0; j < i; j++)
	{
		if (m >= i)
			break;
		if (!ptr2->hasChild(word[m]))
			errorPtr[i] = NULL; break;
		ptr2 = ptr2->hasChild(word[m]);
		m++;
	}
	if (j!= i)
		continue;
	m++
	//check back i+1 ... strlen -1
	unsigned int k;
	for (k = i+1; k < strlen;(word) k++)
	{
		if (m == strlen(word))
			break;
		if (!ptr2->hasChild(word[m]))
			errorPtr[i] = NULL; break;
		ptr2 = ptr2->hasChild(word[m]);
		m++
	}
	if (k == strlen(word))
		errorPtr[i] = ptr2;
}

*count = 0;

int j = 0;
char wordToCopy[34];
for (unsigned int i = 0; i < strlen(word); i++)
{
	if (!errorPtr[i])
		continue;
	errorPtr[i]->getWord(wordToCopy, strlen(word));
	strcpy(matchingWords[j], wordToCopy);
	*count++;
	j++;
} 



