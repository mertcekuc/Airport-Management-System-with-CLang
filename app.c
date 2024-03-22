//Mert Cekuc, 042001022
//Emre Yilmaz.042101110

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLIGHT_No_SIZE 7
#define TIME_SIZE 6
#define DATE_SIZE 11

struct Passenger
{

    char fullName[32];
    char class;
    char flightNo[FLIGHT_No_SIZE];
};

typedef struct Passenger Passenger;

struct Flight
{
    char flightNo[FLIGHT_No_SIZE];
    char destination[17];

    int numEcons;
    int numBus;

    int limitEcon;
    int limitBus;

    char date[DATE_SIZE];
    char time[TIME_SIZE];

    struct Passenger *passengers;
};

typedef struct Flight Flight;

int lineCount(FILE *fptr)
{

    int count = 0;
    char buffer[100];

    fgets(buffer, 100, fptr);

    while (!feof(fptr))
    {
        fgets(buffer, 100, fptr);
        count++;
    }

    return count - 1;
}

void loadFlight(Flight *flights)
{

    FILE *file = fopen("Flights.csv", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Read each line of the file
    char line[100];
    fgets(line, 100, file);

    int count = 0;
    while (fgets(line, 100, file) != NULL)
    {
        // Split the line into fields
        char *field = strtok(line, ",");
        int fieldIndex = 0;

        // Initialize a 2D array to store the fields
        char fields[8][100];
        while (field != NULL)
        {
            strcpy(fields[fieldIndex], field);
            field = strtok(NULL, ",");
            fieldIndex++;
        }

        Flight temp;

        strcpy(temp.flightNo, fields[0]);
        strcpy(temp.destination, fields[1]);
        temp.numEcons = 0;
        temp.numBus = 0;
        temp.limitBus = atoi(fields[4]);
        temp.limitEcon = atoi(fields[5]);
        strcpy(temp.date, fields[6]);
        strcpy(temp.time, fields[7]);

        flights[count] = temp;
        flights[count].passengers=(Passenger*) malloc((flights[count].limitBus+flights[count].limitEcon)* sizeof(Passenger));

        count++;
    }

    fclose(file);
}

void loadPassengers(Passenger* passengers, Flight *flights, int flightSize)
{

    FILE *file = fopen("Passenger.csv", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char line[100];
    fgets(line, 100, file);

    int count = 0;
    while (fgets(line, 100, file) != NULL)
    {
        // Split the line into fields

        char *field = strtok(line, ",");
        int fieldIndex = 0;

        // Initialize a 2D array to store the fields
        char fields[3][100];
        while (field != NULL)
        {
            strcpy(fields[fieldIndex], field);
            field = strtok(NULL, ",");
            fieldIndex++;
        }

        Passenger temp;
        strcpy(temp.fullName, fields[0]);
        temp.class = fields[1][0];
        strcpy(temp.flightNo, fields[2]);
        passengers[count]=temp;

        for (int i = 0; i < flightSize; i++)
        {
            if (strncmp(fields[2], flights[i].flightNo, 6) == 0)
            {
                int passnum = flights[i].numBus + flights[i].numEcons;
                flights[i].passengers[passnum] = passengers[count];
                if (temp.class == 'E')
                    flights[i].numEcons++;

                else
                    flights[i].numBus++;
            }

        }

        count++;
    }

    for(int i=0; i<flightSize;i++){
        flights[i].passengers=(Passenger*) realloc(flights[i].passengers, (flights[i].numBus+flights[i].numEcons)*sizeof(Passenger));
    }
}

void listFlightInfo(Flight *flights, int size)
{

    char flightNumber[7];
    printf("\nEnter Flight Number to Search:");
    scanf("%s", flightNumber);

    int found = 0;
    for (int i = 0; i < size; i++)
    {
        if (strncmp(flightNumber, flights[i].flightNo, strlen(flightNumber)) == 0)
        {
            found = 1;

            printf("\nFlight No:%s\n", flights[i].flightNo);
            printf("Destination:%s\n", flights[i].destination);
            printf("Date:%s %.5s\n", flights[i].date, flights[i].time);
            printf("Passenger List: ");

            for (int k = 0; k < flights[i].numEcons + flights[i].numBus; k++)
            {
                printf("%s, ", flights[i].passengers[k].fullName);
            }

            return;
        }
    }

    if (found == 0)
        printf("Flight can't be found.");
}

void findTicketInfo(Flight *flights, int size)
{

    char name[32];
    printf("Enter Passenger Name:");
    fgets(name, 32, stdin);

    int found = 0;

    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < flights[i].numBus + flights[i].numEcons; k++)
        {
            if (strncmp(name, flights[i].passengers[k].fullName, strlen(flights[i].passengers[k].fullName)) == 0)
            {

                found = 1;
                printf("\nPassenger Name:%s", name);
                printf("\nFlight No:%.6s", flights[i].flightNo);
                printf("\nFlight Destination, Time:%s, %s %.6s", flights[i].destination, flights[i].date, flights[i].time);
                return;
            }
        }
    }

    if (found == 0)
    {
        printf("Passenger not found.");
    }
}

void addPassanger(Flight *flights, int size)
{
    Passenger temp;

    printf("\nEnter flight Num: ");
    scanf("%s", temp.flightNo);

    int c;
    while ((c = getchar()) != '\n' && c != EOF){}

    printf("\nEnter Passenger Name: ");
    fgets(temp.fullName, 32, stdin);
    temp.fullName[strlen(temp.fullName)-1]='\0';


    printf("Enter Passenger Class (E/B): ");
    char class[32];
    scanf("%s", class);
    temp.class = class[0];

    for (int i = 0; i < size; i++)
    {

        if (strcmp(temp.flightNo, flights[i].flightNo) == 0)
        {   
            flights[i].passengers=(Passenger*) realloc(flights[i].passengers, (flights[i].numBus+flights[i].numEcons+1)*sizeof(Passenger));

            int passnum = flights[i].numBus + flights[i].numEcons;
            flights[i].passengers[passnum] = temp;
            if (class[0] == 'B')
            {
                flights[i].numBus++;
            }

            else {
                flights[i].numEcons++;}

            return;
        }
    }
}

void addFlight(Flight *flights, int flightSize)
{

    flights = (Flight *)realloc(flights, (flightSize+1) * sizeof(Flight));

    printf("Enter Flight Code: ");
    scanf("%s", flights[flightSize ].flightNo);

    printf("\nEnter flight destiantion: ");
    scanf("%s", flights[flightSize ].destination);

    printf("\nEnter economy class passenger limit: ");
    scanf("%d", &flights[flightSize ].limitEcon);
    flights[flightSize ].limitBus = 300 - flights[flightSize - 1].limitEcon;

    printf("\nEnter date: ");
    scanf("%s", flights[flightSize].date);

    printf("\nEnter time:");
    scanf("%s", flights[flightSize].time);

    flights[flightSize].numBus = 0;
    flights[flightSize].numEcons = 0;
}

void deletePassenger(Flight *flights, int size)
{
    char name[32];

    printf("Enter name to delete:");
    fgets(name, 32, stdin);

    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < flights[i].numBus+flights[i].numEcons; k++)
        {
            if (strncmp(name, flights[i].passengers[k].fullName, strlen(flights[i].passengers[k].fullName)) == 0)
            {

                char checkClass = flights[i].passengers[k].class;
                for (int j = k; j < flights[i].numBus+ flights[i].numEcons; j++)
                {
                    flights[i].passengers[j] = flights[i].passengers[j + 1];
                }
                if (checkClass == 'B'){
                    flights[i].numBus--;}

                else{
                    flights[i].numEcons--;}


                flights[i].passengers=(Passenger*) realloc(flights[i].passengers, (flights[i].numBus+flights[i].numEcons)*sizeof(Passenger));
                return;
            }
        }
    }

    printf("Passenger not found");
}

void deleteFlight(Flight *flights, int flightsize)
{

    char flightNumber[7];
    printf("Enter flight No to delete: ");
    scanf("%s", flightNumber);

    for (int i = 0; i < flightsize; i++)
    {
        if (strncmp(flightNumber, flights[i].flightNo, strlen(flightNumber)) == 0)
        {
           
            // Shift flights in array
            for (int j = i; j < flightsize; j++)
            {
                if (j != flightsize - 1)
                {
                    flights[j] = flights[j + 1];
                }
            }

            // Decrement flightsize and resize flights array
            flightsize --;
            flights = (Flight *)realloc(flights, flightsize * sizeof(Flight ));

            // Check if realloc was successful
            if (flights == NULL)
            {
                printf("Error resizing flights array!\n");
                return;
            }

            return;
        }
    }

    printf("Flight not found");
}

void saveData(Flight *flights, int size)
{
    FILE *flightptr = fopen("Flights.csv", "w");
    FILE *passengerptr = fopen("Passenger.csv", "w");

    if ((flightptr == NULL) || (passengerptr == NULL))
    {
        printf("FILE ERROR");
        return;
    }

        fprintf(flightptr,"FlightNo,Destination,numEcon,numBus,limitEcon,limitBus,Date,Time\n",300);
        fprintf(passengerptr,"FullName,Class,FlightNo\n",100);

    for (int i = 0; i < size; i++)
    {
        fprintf(flightptr, "%s,%s,%d,%d,%d,%d,%s,%.5s", flights[i].flightNo, flights[i].destination, flights[i].numEcons, flights[i].numBus, flights[i].limitEcon, flights[i].limitBus, flights[i].date, flights[i].time, 300);
        fprintf(flightptr, "\n", 1);

        for (int k = 0; k < (flights[i].numBus + flights[i].numEcons); k++)
        {
            fprintf(passengerptr, "%s,%c,%.6s", flights[i].passengers[k].fullName, flights[i].passengers[k].class, flights[i].passengers[k].flightNo, 128);
            fprintf(passengerptr, "\n", 1);
        }
    }
}

int main()
{

    FILE *flightptr;
    flightptr = fopen("Flights.csv", "r");

    FILE *passegerptr;
    passegerptr = fopen("Passenger.csv", "r");

    if ((flightptr == NULL) || (passegerptr == NULL))
    {
        printf("FILE EROOR");
    }
    else
    {

        int flightSize = lineCount(flightptr);
        int passengerSize = lineCount(passegerptr);
        Flight *flights = (Flight *)malloc(2 * flightSize * sizeof(Flight));
        Passenger* passengers=(Passenger*) malloc(passengerSize * sizeof(Flight));

        loadFlight(flights);
        loadPassengers(passengers, flights, flightSize);



        while (1)
        {
            printf("\n\n1-Display Flight Info\n");
            printf("2-Display Passengre Info\n");
            printf("3-Add Flight\n");
            printf("4-Add Reservation for Passenger\n");
            printf("5-Delete Flight\n");
            printf("6-Delete Reservation\n");
            printf("0-Save and Quit\n");


            int option;
            printf("\n\nOPTION: ");

            scanf("%d", &option);
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }

            if (option == 1)
                listFlightInfo(flights, flightSize);
            else if (option == 2)
                findTicketInfo(flights, flightSize);
            else if (option == 3)
            {
                addFlight(flights, flightSize);
                flightSize++;
            }
            else if (option == 4)
                addPassanger(flights, flightSize);
            else if (option == 5)
            {
                deleteFlight(flights, flightSize);
                flightSize--;
            }
            else if (option == 6)
                deletePassenger(flights, flightSize);
            else if (option == 0)
            {
                saveData(flights, flightSize);
                break;
            }

            else
                printf("Wrong Option");
        }
    }
    return 0;
}
