typedef struct {
	int zipCode;
	char *name;
}

void CreateCity(City *city, int zip, const char *cityName);
int getZipCode(const City *city);
int main()
{
	City city;
	CreateCity(&city, 95616, "Davis");
	cout << getZipCode(&city);
	return 0;
}