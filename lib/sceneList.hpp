/**************************************\
* Некоторое количество хитрых макросов *
* для описания списка сцен             *
\**************************************/

#define scenes Scene* Game::create(string s) {
#define startScene(nm, z) if(s == "" || s == nm) \
                           return new z; \
                         else
#define scene(nm, z) if(s == nm) \
                           return new z; \
                         else
#define thatsall throw "No scene names \"" + s + "\""; }