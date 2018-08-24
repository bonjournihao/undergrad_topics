//city.cpp
//implementation of class methods
#include "city.h"


//default constructor
City::City(): zipCode(0), name(NULL)
{
	//zipCode = 0;
	//name = NULL;
}

//standard constructor
City::City(int zipC, char *nam): zipCode(zipC)
{
	//zipCode = zipC;
	name = new char[strlen(name) + 1];
	name = strcpy(name, nam);

}

//copy constructor
City::City(const City *c)
{
	zipCode = c->zipCode;
	name = new char[strlen(c->name) + 1];
	strcpy(name, c->name)

}

//destructor
City::~City()
{
	//release the memory
	delete name;
}

//functions
void City::CreatCity(City *city, int zip, const char *cityName)
{
}

int City::getZipCode()
{

}