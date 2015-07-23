#include "World.h"
#include "Object.h"
#include "Player.h"

namespace{
	const u8 MAX_BUFFER_SIZE = 64;
	const u8 MAX_PATH_SIZE = 32;
	u16 GetNumCharCount(u8 *src) {
		u16 count = 0;
		while(IS_NUMBER(*src)){
			++count;
			++src;
		}
		return count;
	}
}


//TODO: Add functionality later...
World::World(){

	//GLuint *textures = new GLuint[5]; // [numTextures]
	//glGenTextures(5, textures);
	
	//plane = new Plane(1, 1);

	waterB = new Plane(1, 1);
	
	waterBTexture = waterB->LoadDDS("./textures/waterBlue.DDS");
	waterB->SetPosition(vec3(1.0f, 1.0f, 0.0f)); // MOVES PLAYERTEXTURE, WHY?

	waterLB = new Plane(2, 2);
//	waterLB->SetPosition(vec3(0));
	waterLBTexture = waterLB->LoadDDS("./textures/waterLBlue.DDS");
	
	waterG = new Plane(3, 3);
	
	waterGTexture = waterG->LoadDDS("./textures/waterGreen.DDS");
//	waterG->SetPosition(vec3(0));
	waterLG = new Plane(4, 4);
	
	waterLGTexture = waterLG->LoadDDS("./textures/waterLGreen.DDS");
//	waterLG->SetPosition(vec3(0));
	player = new Player(1, 1);
	
	playerTexture = player->LoadDDS("./textures/wall_512.DDS");//waterBlue.DDS");
//	player->SetPosition(vec3(-1.0f, -1.0f, 0.0f));
	
	
	//glActiveTexture(GL_TEXTURE0);
	
	//glBindTexture(GL_TEXTURE_2D, waterBTexture);
	
	//glActiveTexture(GL_TEXTURE1);
	
	//glBindTexture(GL_TEXTURE_2D, waterLBTexture);
	//glActiveTexture(GL_TEXTURE2);
	
	//glBindTexture(GL_TEXTURE_2D, waterGTexture);
	//glActiveTexture(GL_TEXTURE3);
	
	//glBindTexture(GL_TEXTURE_2D, waterLGTexture);
	
			//
			//textures[2] = waterLB->LoadDDS("./textures/waterLBlue.DDS");
		//	glBindTexture(GL_TEXTURE_2D, textures[2]);
			//
			//textures[3] = waterG->LoadDDS("./textures/waterGreen.DDS");
		//	glBindTexture(GL_TEXTURE_2D, textures[3]);
			//
			//textures[4] = waterLG->LoadDDS("./textures/waterLGreen.DDS");
			//glBindTexture(GL_TEXTURE_2D, textures[4]);

	//waterB->textureID = waterB->LoadDDS("./textures/waterBlue.DDS");
			
	//glBindTexture(GL_TEXTURE_2D, waterB->textureID); 

	//GLuint textureID = plane->LoadBMP("./Moss_128.bmp");
	//textures[0] = plane->LoadDDS("./textures/wall_512.DDS");
	//glBindTexture(GL_TEXTURE_2D, textures[0]); 
	

	//load world...
	ifstream myfile(LEVEL_0);

	char buffer[::MAX_BUFFER_SIZE];
	char path[::MAX_PATH_SIZE];

	if (myfile.is_open())
	{
		string line;
		unsigned char len = 0;

		bool loadLevelData = true;

		while ( getline (myfile,line) )
		{
			line.copy(buffer, len = line.length());
			buffer[len] = '\0';

			if(IS_EMPTY_LINE(buffer[0])){
				continue;
			}
			else if(LOAD_TEXTURE(buffer[0])){
				//Setting buffer size...
				numTextures = ASCII_ZERO - buffer[1];
				textureBuffer = (u8*)malloc(MAX_PATH_SIZE * numTextures);

				loadLevelData = false;
				continue;
			}
			else if(LOAD_LEVELDATA(buffer[0])){
				//Setting buffer size... and adding 1 to accomindate for comma...
				u8 numStride = ::GetNumCharCount((u8*)&buffer[1]) + 1;

				levelWidth = std::stoi(&buffer[1]);// - ASCII_ZERO;
				levelHeight = std::stoi(&buffer[1 + numStride]);//buffer[3] - ASCII_ZERO;
				

				levelBuffer = (u8*)malloc(levelWidth * levelHeight);

				loadLevelData = true;
				continue;
			}
			else if(loadLevelData){
				//TODO: Load level data here...

				static u16 levelIndex = 0;
				//u8 count = 0;

				for(u16 i = 0, count = 0; count < levelWidth; ++i) {
					if(IS_NUMBER(buffer[i])){
						levelBuffer[levelIndex++] = buffer[i];
						++count;
					}
				}

				continue;
			}

			unsigned char commaLen = FindChar(buffer, ',');

			static u8 texIndex = 0;
			//Found textures...
			if(commaLen > 0){
				//strcpy_s(path, commaLen, buffer);
				strcpy(path, buffer);
				path[commaLen - 1] = '\0';
				memcpy(&textureBuffer[texIndex], &path, MAX_PATH_SIZE);
				texIndex += MAX_PATH_SIZE - 1;

				continue;
			}

			//load textures...

			//string filePath = 
		}

		myfile.close();
	}

	else cout << "Unable to open file"; 
}

unsigned char World::FindChar(const char* buffer, const char& c){
	//TODO: Search for char c, if found return true...
	char* value = (char*)buffer;
	unsigned char len = 0;

	while(value != '\0'){
		++len;
		if(*value == c){
			return len;
		}
		++value;
	}

	return len = 0;
}

//TODO: Add functionality later...
World::~World(){
	delete waterB;
	waterB = NULL;
	delete waterLB;
	waterLB = NULL;
	delete waterG;
	waterG = NULL;
	delete waterLG;
	waterLG = NULL;
	delete player;
	player = NULL;
}

void World::Update(const float& deltaTime){

//	plane->Update(deltaTime);
	
	waterB->Update(deltaTime);
	waterG->Update(deltaTime);
	waterLB->Update(deltaTime);
	waterLG->Update(deltaTime);
	player->Update(deltaTime);
}

void World::Render(const Camera& camera){
	/**/
	glBindTexture(GL_TEXTURE_2D, waterLGTexture);
	
	waterLG->Render(camera);
	glBindTexture(GL_TEXTURE_2D, waterGTexture);
	waterG->Render(camera);
	
	

	glBindTexture(GL_TEXTURE_2D, waterLBTexture);
	waterLB->Render(camera);
	glBindTexture(GL_TEXTURE_2D, playerTexture);
	player->Render(camera);
	


	glBindTexture(GL_TEXTURE_2D, waterBTexture);
	waterB->Render(camera);
	
	
	
	//waterG->Render(camera);
}