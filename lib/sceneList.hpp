/**************************************\
* Некоторое количество хитрых макросов *
* для описания списка сцен             *
\**************************************/

#define scenes Scene* SceneManager::create(string s) {
#define startScene(z) if(s == "" || s == typeid(z).name()) \
                           return new z; \
                         else
#define scene(z) if(s == typeid(z).name()) \
                           return new z; \
                         else
#define thatsall throw "No scene \"" + s + "\""; }