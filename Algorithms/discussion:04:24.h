//city.h
//declaration of class City
//by default, the member variables and functions are private;

class City{
	//member variables
	int zipCode;
	char *name;
	void privateFunc();

private:
	//member functions

	//constructor (no return, same name as the class)
	City(); //default
	City(int, char *); //standard
	City(const City *c); //copy constructor

	//destructor (only one)
	~City();


	void CreateCity(City *city, int zip, const char *cityName);
	int getZipCode();
};


//main function
void main()
{
	City yourCity(95616, "Davis");//will also call standard constructor
	City myCity(yourCity);//copy constructor
	//City(); wrong;
	//yourCity.City(); wrong

	delete yourCity;

	City anotherCity;
	for (int i = 0; i < 10; i++)
	{
		City newCity;
		anotherCity->getZipCode();

	} //~newCity

	//newCity will be destroyed at the end of for loop
	//anotherCity is declared outside the for loop, therefore won't be destroyed

}// ~anotherCity ~myCity