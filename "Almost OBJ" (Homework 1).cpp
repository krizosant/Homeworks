// Тримерен модел ще наричаме съвкупност от точки в тримерното пространство и съвкупност от многоъгълници с върхове измежду дадените върхове.

// Често използван формат за съхраняване на такива обекти във файл е Wavefront (.obj). Файл от този тип е текстов и се състои три типа редове:
// ‘v <x> <y> <z>’ - описва връх в модела. <x>, <y> и <z> са рационални числа, записани в текстов вид (10.001, 123.045, 2, -5.000).
// ‘f <v1> <v2> … <vn>’ - описва многоъгълник от повърхността на модела. <v1>, <v2>, …, <vn> са индексите на върховете, които участват в многоъгълника (цели положителни числа).
// ‘# … ’ - коментар; Всеки ред, започващ с ‘#’ се игнорира при четене от файл.
// Такива файлове може да визуализирате чрез приложението 3D Viewer в Windows 10. Друга добра опция е

// Задание:
// Реализирайте клас Object3d, който представя тримерен модел в езика C++.
// Реализирайте конструктор, който приема име на файл и зарежда модел от него.
// Реализирайте конструктор, който приема поток за вход и зарежда модел от него.
// Реализирайте методи ‘int getVertexCount()’ и ‘int getFaceCount()’, които връщат съответно броя върхове и броя многоъгълници в модел.
// Реализирайте метод ‘void save(const string& file)’, който приема име на файл и съхранява модела във файла.
// Реализирайте метод ‘void print(ostream &)’, който приема изходен поток, в който да се запишат данните на обекта в указания формат.
// Реализирайте метод ‘Object3d cut(function<bool>(float x, float y, float z) f)’, който връща модел съставен само от тези многоъгълници в модела, координатите на чиито върхове отговарят на предиката ‘f’.
// Реализирайте метод ‘void flip()’, който обръща всички многоъгълници в модела наобратно. (Четириъгълникът 1 2 3 4 става 4 3 2 1)
// Реализирайте статични методи за създаване на куб и сфера с параметри съответно дължина на ръб и радиус. (Не е задължително сферата да изглежда точно като на картинката по-горе)
// При опит за прочитане на невалиден файл изведете подходящи съобщения.


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <functional>

struct Point
{
	char indicator;
	float x, y, z;
};

class Model
{
private:
	std::fstream memberFile;
	std::vector<std::string> comments;
	std::vector<Point> points;
	std::vector<std::vector<int>> faces;


public:
	Model(const std::string& file)
	{
		std::string line, tempStr;
		size_t index, stringIndex, positionCounter;
		int numberOfFaces;

		memberFile.open(file, std::ios::in);
		points.reserve(getVertexCount());

		memberFile.open(file, std::ios::in);
		faces.reserve(getFaceCount());

		memberFile.open(file, std::ios::in);

		index = 0;
		numberOfFaces = getFaceCount();
		while(!memberFile.eof())
		{
			getline(memberFile, line);
			if(line[0] == 'v')
			{
				points[index].indicator = 'v';
				stringIndex = 1, positionCounter = 1;
				do
				{
					if(((line[stringIndex] >= '0') && (line[stringIndex] <= '9')) || (line[stringIndex] == '-'))
					{
						if(line[stringIndex] == '0')
						{
							if(positionCounter == 1)
							{
								points[index].x = getNumber(line, &stringIndex);
								(stringIndex)++;
								positionCounter++;
							}
							else if(positionCounter == 2)
							{
								points[index].y = getNumber(line, &stringIndex);
								(stringIndex)++;
								positionCounter++;
							}
							else
							{
								points[index].z = getNumber(line, &stringIndex);
								(stringIndex)++;
								index++;
							}
						}
						else if(line[stringIndex-1] == '-')
						{
							//tempStr = line[stringIndex];
							if(positionCounter == 1)
							{
								points[index].x = 0 - getNumber(line, &stringIndex);
								stringIndex++;
								positionCounter++;
							}
							else if(positionCounter == 2)
							{
								points[index].y = 0 - getNumber(line, &stringIndex);
								stringIndex++;
								positionCounter++;
							}
							else
							{
								points[index].z = 0 - getNumber(line, &stringIndex);
								stringIndex++;
								index++;
							}
						}
						else
						{
							if(positionCounter == 1)
							{
								points[index].x = getNumber(line, &stringIndex);
								stringIndex++;
								positionCounter++;
							}
							else if(positionCounter == 2)
							{
								points[index].y = getNumber(line, &stringIndex);
								stringIndex++;
								positionCounter++;
							}
							else
							{
								points[index].z = getNumber(line, &stringIndex);
								stringIndex++;
								index++;
							}
						}
					}
					else
					{
						stringIndex++;
					}
				}
				while(stringIndex < line.size());
			}
			else if(line[0] == 'f')
			{
				for(int indexRow = 0; indexRow < numberOfFaces; indexRow++)
				{
					std::vector<int> newVec;
					for(size_t indexCol = 0; indexCol < line.size(); indexCol++)
					{
						if((line[indexCol] >= '0') && (line[indexCol] <= '9'))
						{
							newVec.push_back(102);
							tempStr = line[indexCol];
							newVec.push_back(stoi(tempStr));
						}
					}
					faces.push_back(newVec);
					getline(memberFile, line);
				}
			}
			else
			{
				comments.push_back(line);
				comments.push_back(" ");
			}
		}
		memberFile.close();
	}
	Model(std::ifstream& objFile)
	{
		std::string line, tempStr;
		size_t index, stringIndex, positionCounter, numberOfFaces;

		numberOfFaces = getFaceCount();
		points.reserve(getVertexCount());
		faces.reserve(getFaceCount());

		index = 0;
		while(!memberFile.eof())
		{
			getline(memberFile, line);
			if(line[0] == 'v')
			{
				stringIndex = 1, positionCounter = 1;
				do
				{
					if(((line[stringIndex] >= '0') && (line[stringIndex] <= '9')) || (line[stringIndex] == '-'))
					{
						if(line[stringIndex] == '0')
						{
							if(positionCounter == 1)
							{
								points[index].x = getNumber(line, &stringIndex);
								(stringIndex)++;
								positionCounter++;
							}
							else if(positionCounter == 2)
							{
								points[index].y = getNumber(line, &stringIndex);
								(stringIndex)++;
								positionCounter++;
							}
							else
							{
								points[index].z = getNumber(line, &stringIndex);
								(stringIndex)++;
								index++;
							}
						}
						else if(line[stringIndex-1] == '-')
						{
							//tempStr = line[stringIndex];
							if(positionCounter == 1)
							{
								points[index].x = 0 - getNumber(line, &stringIndex);
								stringIndex++;
								positionCounter++;
							}
							else if(positionCounter == 2)
							{
								points[index].y = 0 - getNumber(line, &stringIndex);
								stringIndex++;
								positionCounter++;
							}
							else
							{
								points[index].z = 0 - getNumber(line, &stringIndex);
								stringIndex++;
								index++;
							}
						}
						else
						{
							if(positionCounter == 1)
							{
								points[index].x = getNumber(line, &stringIndex);
								stringIndex++;
								positionCounter++;
							}
							else if(positionCounter == 2)
							{
								points[index].y = getNumber(line, &stringIndex);
								stringIndex++;
								positionCounter++;
							}
							else
							{
								points[index].z = getNumber(line, &stringIndex);
								stringIndex++;
								index++;
							}
						}
					}
					else
					{
						stringIndex++;
					}
				}
				while(stringIndex < line.size());
			}
			else if(line[0] == 'f')
			{
				for(size_t indexRow = 0; indexRow < numberOfFaces; indexRow++)
				{
					std::vector<int> newVec;
					for(size_t indexCol = 0; indexCol < line.size(); indexCol++)
					{
						if(((line[indexCol] >= '0') && (line[indexCol] <= '9')) || (line[indexCol] == ' '))
						{
							tempStr = line[indexCol];
							newVec.push_back(stoi(tempStr));
						}
					}
					faces.push_back(newVec);
					getline(objFile, line);
				}
			}
			else
			{
				comments.push_back(line);
				comments.push_back(" ");
			}
		}
		objFile.close();
	}
	Model(){} //Default Constructor
	int getVertexCount() //Работи; Извиква се на ред 35
	{
		size_t counter = 0;
		std::string str;

		do
		{
			std::getline(memberFile, str);
			if(str[0] == 'v')
			{
				counter++;
			}
		}
		while(!memberFile.eof());

		memberFile.close();

		return counter;
	}
	int getFaceCount() //Работи; Извиква се на ред 38
	{
		size_t counter = 0;
		std::string str;

		do
		{
			std::getline(memberFile, str);
			if(str[0] == 'f')
			{
				counter++;
			}

		}
		while(!memberFile.eof());

		memberFile.close();

		return counter;
	}
	void save(const std::string& file)
	{
		std::ofstream outputFile(file);

		for(const std::string& comment : comments)
		{
			outputFile << comment << std::endl;
		}

		for(const Point& point : points)
		{
			outputFile << point.indicator << " " << point.x << " " << point.y << " " << point.z << std::endl;
		}

		for (const std::vector<int>& f : faces)
		{
			outputFile << "f";
			for (int v : f)
			{
				outputFile << " " << v;
			}
			outputFile << std::endl;
		}

		outputFile.close();
	}
	void flip()
	{
		reverse(faces.begin(), faces.end());
	} 
	float getNumber(std::string str, size_t* index)
	{
		std::string tempStr;
		do
		{
			if(((str[*index] >= '0') && (str[*index] <= '9')) || (str[*index] == '-') || (str[*index] == '.'))
			{
				tempStr += str[*index];
				(*index)++;
			}
			else
			{
				(*index)++;
			}

		}
		while(((str[*index] >= '0') && (str[*index] <= '9')) || (str[*index] == '-') || (str[*index] == '.'));

		return stof(tempStr);
	}
};

int main()
{
	Model DefaultModel;

	Model firstModel("sphere.obj");
	firstModel.flip();

	std::ifstream myFirstFile;
	myFirstFile.open("bunny.obj");
	if(myFirstFile.is_open())
		Model secondModel(myFirstFile);

	Model myThirdModel("cube.obj");
	myThirdModel.save("ModelCopyFile.obj");

	return 0;
}
