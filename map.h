#include <SFML/Graphics.hpp>

using namespace sf;



const int width = 640;
const int height = 480;

const int HEIGHT_MAP = 20;
const int WIDTH_MAP = 38;

const int STEP = 32;


const String TILEMAP[HEIGHT_MAP] = {
	"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
	"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
	"bb                                  bb",
	"bb                                  bb",
	"bb                                  bb",
	"bb       b                          bb",
	"bb   b b b                          bb",
	"bb   b   b                          bb",
	"bb                         bbbbb    bb",
	"bb                         b        bb",
	"bb                         bbbbb    bb",
	"bb                        b         bb",
	"bb                                  bb",
	"bb                                  bb",
	"bb              b                   bb",
	"bb              b                   bb",
	"bb              b                   bb",
	"bb              b                   bb",
	"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
	"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
};

