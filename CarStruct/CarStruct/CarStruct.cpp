/******************************************************************************
Павлов Владислав Евгеньевич
КТбо1-2
Лабораторная работа №5 «Файлы данных»
Вариант № 7
Задание: Написать программу, которая записывает с клавиатуры в файл структуру согласно выданному варианту задания.
         В качестве разделителя полей структуры использовать символ табуляции. В программе реализовать:
         а) дополнение  существующего массива структур новыми структурами;
         б) поиск структуры с заданным значением выбранного элемента;
         в) вывод на экран содержимого массива структур;
         г) упорядочение массива структур по заданному полю (элементу), например государство по численности.
*******************************************************************************/

#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
//#define DEBUG
#define COUNT_CAR_PROPERTYS 8

struct Car
{
    std::string mark,
        color,
        serialNumber,
        regNumber,
        carType;
    int cost, doorsCount;
    std::tm releaseDate;
};

void show_car_info(const Car car);

void fill_car_info(Car& car);

bool is_car_property(const std::string property);

void show_cars_by_property(const std::vector<Car> cars, const std::string property, const std::string typeOfProperty);

void fill_time_from_string(const std::string dateStr, std::tm& date);

void read_cars_from_file(const std::string filePath, std::vector<Car>& vectorForCars);

void fill_car_in_file(const Car car, std::string beautyCarInfoPath, std::string carInfoPath);

void sort_cars_by(std::vector<Car>& cars, const std::string typeOfProperty, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate);

void add_new_cars(const std::string beautyCarInfoPath, const std::string carInfoPath);

void find_a_cars_with_your_property(const std::vector<Car> cars);

void show_all_cars(const std::vector<Car> cars);

void sort_cars(std::vector<Car>& cars, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate);

void sort_by_cost(std::vector<Car>& cars, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate);

void sort_by_doors_count(std::vector<Car>& cars, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate);

void sort_by_release_date(std::vector<Car>& cars, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate);

int main()
{
    bool sortedByCost, sortedByDoorsCount, sortedByReleaseDate, weAreAlive = true;
    sortedByCost = sortedByDoorsCount = sortedByReleaseDate = false;
    while (weAreAlive)
    {
        int carsCount;
        const std::string BEAUTY_CAR_INFO_PATH = "b_cars.txt";
        const std::string CAR_INFO_PATH = "cars.txt";
        std::vector<Car> cars;
        int choice;
        read_cars_from_file(CAR_INFO_PATH, cars);
        if (sortedByCost)
        {
            sort_by_cost(cars, sortedByCost, sortedByDoorsCount, sortedByReleaseDate);
        }
        if (sortedByDoorsCount)
        {
            sort_by_doors_count(cars, sortedByCost, sortedByDoorsCount, sortedByReleaseDate);
        }
        if (sortedByReleaseDate)
        {
            sort_by_release_date(cars, sortedByCost, sortedByDoorsCount, sortedByReleaseDate);
        }
        std::cout << "What do you want?\n1 - add new cars\n2 - find a cars with your property\n3 - show all cars\n4 - sort a cars with your property\n5 - exit\n";
        std::cin >> choice;
        system("cls");
        switch (choice)
        {
        case 1:
            add_new_cars(BEAUTY_CAR_INFO_PATH, CAR_INFO_PATH);
            system("cls");
            cars.clear();
            read_cars_from_file(CAR_INFO_PATH, cars);
            break;
        case 2:
            find_a_cars_with_your_property(cars);
            system("cls");
            break;
        case 3:
            show_all_cars(cars);
            system("cls");
            break;
        case 4:
            sort_cars(cars, sortedByCost, sortedByDoorsCount, sortedByReleaseDate);
            system("cls");
            break;
        case 5:
            weAreAlive = false;
            break;
#ifdef DEBUG
        case 6:
            std::cout << cars.size() << std::endl;
            system("pause");
            system("cls");
            break;
#endif // DEBUG 
        default:
            std::cout << "wrong choice";
            break;
        }
    }
}

bool time_equal(std::tm date1, std::tm date2)
{
    return date1.tm_year == date2.tm_year && date1.tm_mon == date2.tm_mon && date1.tm_mday == date2.tm_mday;
}

void show_cars_by_property(const std::vector<Car> cars, const std::string property, const std::string typeOfProperty)
{
    std::tm date;
    fill_time_from_string(property, date);
    for (Car car : cars)
    {
        if (car.carType == property && typeOfProperty == "type")
            show_car_info(car);
        else if (car.color == property && typeOfProperty == "color")
            show_car_info(car);
        else if (typeOfProperty == "cost")
        {
            if (car.cost == stoi(property))
                show_car_info(car);
        }
        else if (typeOfProperty == "doors count")
        {
            if(car.doorsCount == stoi(property))
                show_car_info(car);
        }
        else if (car.mark == property && typeOfProperty == "mark")
            show_car_info(car);
        else if (car.regNumber == property && typeOfProperty == "registration number")
            show_car_info(car);
        else if (car.serialNumber == property && typeOfProperty == "serial number")
            show_car_info(car);
        else if (typeOfProperty == "release date")
        {
            if(time_equal(car.releaseDate, date))
                show_car_info(car);
        }
    }
}

void fill_time_from_string(const std::string dateStr, std::tm& date)
{
    std::istringstream ss(dateStr);
    ss >> std::get_time(&date, "%d.%m.%Y");
}

void read_cars_from_file(const std::string filePath, std::vector<Car>& vectorForCars)
{
    std::fstream file;
    file.open(filePath, std::fstream::app | std::fstream::in);
    if (!file.is_open())
    {
        std::cout << "File is not open!";
        return;
    }
    std::string line;
    while (!file.eof())
    {
        file >> line;
        if (!line.empty())
        {
            std::string temp;
            Car car;
            int carPropertyIndex = 0;
            for (int i = 0; i < line.size(); ++i)
            {
                if (line[i] == ',')
                {
                    switch (carPropertyIndex)
                    {
                    case 0:
                        car.mark = temp;
                        temp = "";
                        break;
                    case 1:
                        car.color = temp;
                        temp = "";
                        break;
                    case 2:
                        car.serialNumber = temp;
                        temp = "";
                        break;
                    case 3:
                        car.regNumber = temp;
                        temp = "";
                        break;
                    case 4:
                        car.doorsCount = stoi(temp);
                        temp = "";
                        break;
                    case 5:
                        fill_time_from_string(temp, car.releaseDate);
                        temp = "";
                        break;
                    case 6:
                        car.carType = temp;
                        temp = "";
                        break;
                    case 7:
                        car.cost = stoi(temp);
                        break;
                    default:
                        break;
                    }
                    
                    ++carPropertyIndex;
                    continue;
                }
                temp = temp + line[i];
            }
            if (!line.empty())
            {
                car.cost = stoi(temp);
            }

            vectorForCars.push_back(car);
        }
    }
    file.close();

}

void fill_car_in_file(const Car car, std::string beautyCarInfoPath, std::string carInfoPath)
{
    std::fstream file;
    file.open(beautyCarInfoPath, std::fstream::app);
    if (!file.is_open())
    {
        std::cout << "File is not open!\n";
        return;
    }
    file << "mark: " << car.mark << '\n' << "color: " << car.color << '\n' << "serial number: " << car.serialNumber << '\n'
        << "registration number: " << car.regNumber << '\n' << "doors count: " << car.doorsCount << '\n' 
        << "release date is " << car.releaseDate.tm_mday << '.' << car.releaseDate.tm_mon + 1 << '.' << car.releaseDate.tm_year + 1900 << 
        '\n' << "type: " << car.carType << '\n' << "cost: " << car.cost << '$' << '\n' << '\n';
    file.close();
    file.open(carInfoPath, std::fstream::app);
    if (!file.is_open())
    {
        std::cout << "File is not open!\n";
        return;
    }
    file << car.mark << ',' << car.color << ',' << car.serialNumber << ',' << car.regNumber << ','
        << car.doorsCount << ',' << car.releaseDate.tm_mday << '.' << car.releaseDate.tm_mon + 1 
        << '.' << car.releaseDate.tm_year + 1900 << ',' << car.carType << ',' << car.cost << '\n';
    file.close();
}

void sort_cars_by(std::vector<Car>& cars, const std::string typeOfProperty, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate)
{
    if (typeOfProperty == "cost")
    {
        sort_by_cost(cars, sortedByCost, sortedByDoorsCount, sortedByReleaseDate);
        sortedByCost = true;
    }
    else if (typeOfProperty == "doors count")
    {
        sort_by_doors_count(cars, sortedByCost, sortedByDoorsCount, sortedByReleaseDate);
        sortedByDoorsCount = true;
    }
    else if (typeOfProperty == "release date")
    {
        sort_by_release_date(cars, sortedByCost, sortedByDoorsCount, sortedByReleaseDate);
        sortedByReleaseDate = true;
    }
    else
    {
        std::cout << "incorrect property!\n";
    }


}

void add_new_cars(const std::string beautyCarInfoPath, const std::string carInfoPath)
{
    int carsCount;
    std::cout << "enter the number of cars you want to add: ";
    std::cin >> carsCount;
    if (carsCount < 0)
    {
        std::cout << "Incorrect cars count";
        system("pause");
        system("cls");
    }
    else if (carsCount > 0)
    {
        for (int i = 0; i < carsCount; ++i)
        {
            Car car;
            fill_car_info(car);
            fill_car_in_file(car, beautyCarInfoPath, carInfoPath);
            system("cls");
        }
    }
}

void find_a_cars_with_your_property(const std::vector<Car> cars)
{
    std::string typeOfProperty;
    std::cout << "by which property you want to find the car\n(mark, color, serial number, registration number, doors count, release date, type, cost):\n";
    std::cin.ignore();
    std::getline(std::cin, typeOfProperty);
    if (!is_car_property(typeOfProperty))
    {
        std::cout << "incorrect property\n";
    }
    else
    {
        std::cout << "Enter car " << typeOfProperty << ": ";
        std::string property;
        std::getline(std::cin, property);
        system("cls");
        show_cars_by_property(cars, property, typeOfProperty);
    }
    system("pause");
}

void show_all_cars(const std::vector<Car> cars)
{
    int choice;
    std::cout << "Do you want see all cars?(1 - yes I do; 2 - no I didn't)\n";
    std::cin >> choice;
    if (choice == 1)
    {
        for (Car car : cars)
        {
            show_car_info(car);
        }
        system("pause");
    }
}

void sort_cars(std::vector<Car>& cars, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate)
{
    std::string typeOfProperty;
    std::cout << "By what you want to sort the array of cars?(cost, doors count, release date)\n";
    std::cin.ignore();
    std::getline(std::cin, typeOfProperty);
    sort_cars_by(cars, typeOfProperty, sortedByCost, sortedByDoorsCount, sortedByReleaseDate);
}

void sort_by_cost(std::vector<Car>& cars, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate)
{
    std::sort(cars.begin(), cars.end(), [&](Car car1, Car car2)
        {
            return car1.cost < car2.cost;
        });
    sortedByDoorsCount = sortedByReleaseDate = false;
}

void sort_by_doors_count(std::vector<Car>& cars, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate)
{
    std::sort(cars.begin(), cars.end(), [&](Car car1, Car car2)
        {
            return car1.doorsCount < car2.doorsCount;
        });
    sortedByCost = sortedByReleaseDate = false;
}

void sort_by_release_date(std::vector<Car>& cars, bool& sortedByCost, bool& sortedByDoorsCount, bool& sortedByReleaseDate)
{
    std::sort(cars.begin(), cars.end(), [&](Car car1, Car car2)
        {
            if (car1.releaseDate.tm_year == car2.releaseDate.tm_year)
            {
                if (car1.releaseDate.tm_mon == car2.releaseDate.tm_mon)
                {
                    return car1.releaseDate.tm_mday < car2.releaseDate.tm_mday;
                }
                else
                {
                    return car1.releaseDate.tm_mon < car2.releaseDate.tm_mon;
                }
            }
            return car1.releaseDate.tm_year < car2.releaseDate.tm_year;
        });
    sortedByCost = sortedByDoorsCount = false;
}

void show_car_info(const Car car)
{
    printf("mark = %s\ncolor = %s\nserial number = %s\nregistration number = %s\ndoor count = %d\n%release date = %d/%d/%d\ncar type = %s\ncar cost = %d$\n\n",
        car.mark.c_str(), car.color.c_str(), car.serialNumber.c_str(), car.regNumber.c_str(), car.doorsCount, car.releaseDate.tm_mday, car.releaseDate.tm_mon + 1, car.releaseDate.tm_year + 1900, car.carType.c_str(), car.cost);
}

void fill_car_info(Car& car)
{
    bool allIsCorrect = false;
    while (!allIsCorrect)
    {
        std::cout << "Enter car mark: ";
        std::cin >> car.mark;
        std::cout << "Enter car color: ";
        std::cin >> car.color;
        std::cout << "Enter car serial number: ";
        std::cin >> car.serialNumber;
        std::cout << "Enter registration number: ";
        std::cin >> car.regNumber;
        std::cout << "Enter door count: ";
        std::cin >> car.doorsCount;
        if (car.doorsCount != 2 && car.doorsCount != 4)
        {
            std::cout << "incorrect doors count!(2 or 4)";
            system("pause");
            system("cls");
            continue;
        }
        std::cout << "Enter release date(day.month.year): ";
        std::string releaseDate;
        std::cin >> releaseDate;
        fill_time_from_string(releaseDate, car.releaseDate);
        if (car.releaseDate.tm_year + 1900 < 0 || car.releaseDate.tm_mday <= 0 || car.releaseDate.tm_mon + 1 < 0)
        {
            std::cout << "incorrect date!";
            system("pause");
            system("cls");
            continue;
        }
        std::cout << "Enter car type: ";
        std::cin >> car.carType;
        if (!(car.carType == "light" || car.carType == "jeep" || car.carType == "bus" || car.carType == "truck"))
        {
            std::cout << "incorrect car type!(you can use only light, jeep, bus, truck)";
            system("pause");
            system("cls");
            continue;
        }
        std::cout << "Enter car cost: ";
        std::cin >> car.cost;
        if (car.cost <= 0)
        {
            std::cout << "incorrect cost!";
            system("pause");
            system("cls");
            continue;
        }
        allIsCorrect = true;
    }
    
}

bool is_car_property(const std::string property)
{
    if (property == "mark")
        return true;
    else if (property == "color")
        return true;
    else if (property == "color")
        return true;
    else if(property == "serial number")        
        return true;
    else if(property == "registration number")  
        return true;
    else if(property == "doors count")           
        return true;
    else if(property == "release date")         
        return true;
    else if(property == "type")                 
        return true;
    else if(property == "cost")                 
        return true;
    return false;
}
