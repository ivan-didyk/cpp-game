#pragma once
#include "lib/object.h"
#include "lib/rapidxml/rapidxml.hpp"
#include "lib/rapidxml/rapidxml_utils.hpp"
#include "lib/utf8.hpp"
#include <iostream>
#include <map>
#include <ncurses.h>
#include <cctype>
using namespace std;
using namespace rapidxml;

unsigned getChildCount(xml_node<> *n) {
  unsigned c = 0;
  for (xml_node<> *child = n->first_node(); child; child = child->next_sibling()) {
    c++;
  } 
  return c;
} 

struct Frame {
  u32string * chars;
  char** collmap;
  char** color;
  short height;
  short width;
  short pivotX = 0;
  short pivotY = 0;
};

struct Anim {
  Frame* frames;
  unsigned duration;
};

struct Rect {
  Vector2 topLeft;
  Vector2 bottomRight;
};

class Sprite : public Render {
 private:
  unsigned framesPerDraw = 20;
  unsigned bf = 9;
  Transform* transform;
  map<string, Anim> animations;
  string anim = "default";
  size_t currentFrame = 0;
  bool active = false;
  string filename;

  inline int encode(char ch) {
    chtype color = 0;
    switch(tolower(ch)) {
      case 'b': return COLOR_BLACK;   break;
      case 'r': return COLOR_RED;     break;
      case 'g': return COLOR_GREEN;   break;
      case 'y': return COLOR_YELLOW;  break;
      case 'l': return COLOR_BLUE;    break;
      case 'p': return COLOR_MAGENTA; break;
      case 'c': return COLOR_CYAN;    break;
      case 'w': return COLOR_WHITE;   break;
      default: return COLOR_WHITE; break;
    }
    color = NCURSES_BITS(1U, color);
    if(isupper(ch)) color |= A_BOLD; // Make bright
    return color;
  }

  void draw() {
    Frame& frame = animations[anim].frames[currentFrame];
    for(short i = 0; i < frame.height; ++i) {
      for(short j = 0; j < frame.width; ++j) {
        if(frame.color[i][j] == ' ') {
          continue;
        }
        if(
          i+transform->y()+1-frame.pivotY < 0 ||
          j+transform->x()+1-frame.pivotX < 0 ||
          j+transform->x()+1-frame.pivotX > Game::getScreenSize().x ||
          i+transform->y()+1-frame.pivotY > Game::getScreenSize().y
          ) continue;
        move(i+transform->y()-frame.pivotY, j+transform->x()-frame.pivotX);
        string chars = to_utf8(u32string(1, frame.chars[i][j]));
        char ch = frame.color[i][j];
        int color = encode(ch);
        attron(COLOR_PAIR(color));
        if(islower(ch)) attron(A_DIM           );

        printw("%s", chars.c_str());

        attroff(COLOR_PAIR(color));
        if(islower(ch)) attroff(A_DIM           );
      }
    }
  }
 public:
  Sprite(string name) {
    filename = name;
    bf = framesPerDraw - 1;
  }

  void start() {
    file<char> original(filename.c_str());
    xml_document<char> doc;
    doc.parse<0>(original.data());

    xml_node<> *mainNode = doc.first_node("sprite");


    for(xml_node<> *animation = mainNode->first_node("animation"); animation; animation = animation->next_sibling("animation")) {
      // Parsing animation
      auto name = animation->first_attribute("name");
      if(!name) throw "Not named animation";

      Frame* frames;
      unsigned length = getChildCount(animation);
      frames = new Frame[length];
      unsigned frameIndex = 0;

      for(xml_node<> *frame = animation->first_node("frame"); frame; frame = frame->next_sibling("frame")) {
        // Parsing frame
        Frame current;

        current.height = getChildCount(frame);
        current.color = new char*[current.height];
        current.chars = new u32string[current.height];
        current.collmap = new char*[current.height];
        if(frame->first_attribute("pivotX")) current.pivotX = atoi(frame->first_attribute("pivotX")->value());
        if(frame->first_attribute("pivotY")) current.pivotY = atoi(frame->first_attribute("pivotY")->value());

        bool widthSet = false;
        unsigned width = 0;
        unsigned rowIndex = 0;


        for(xml_node<> *row = frame->first_node("row"); row; row = row->next_sibling("row")) {
          // Getting color information
          auto colorElement = row->first_attribute("color");
          if(!colorElement) throw "Missing color attribute for row";
          if(!widthSet) {
            width = colorElement->value_size();
            widthSet = true;
          }

          // A bit of error checking
          if(colorElement->value_size() != width) throw "Sprite is not rectangle";

          current.color[rowIndex] = new char[width];
          copy(colorElement->value(),
              colorElement->value() + width,
              current.color[rowIndex]);

          auto colElement = row->first_attribute("collision");
          if(colElement) {
            if(colElement->value_size() != width) throw "Collision map is bad width";
            current.collmap[rowIndex] = new char[width];
            copy(colElement->value(),
                colElement->value() + width,
                current.collmap[rowIndex]);
          }
          
          
          // Getting chars
          auto chars = row->first_attribute("chars");

          if(!chars) throw "Missing chars attribute for row";
          if(utf8size(chars->value()) != width) throw "Chars attr is not equal length to color";

          current.chars[rowIndex] = to_utf32(string(chars->value(), chars->value_size()));

          ++rowIndex;
        }

        current.width = width;
        frames[frameIndex] = current;
        ++frameIndex;
      }

      animations[name->value()] = { frames, length };
    }

    transform = getComponent<Transform>(); // I ^o^ Unity
  }

  void startPlaying() { active = true; }
  void stopPlaying() { active = false; }
  void setTrack(string name) { anim = name; currentFrame = 0; }

  void update() {
    ++bf;
    if(bf != framesPerDraw) return;
    bf = 0;
    if(animations.find(anim) == animations.end()) return;
    if(active) {
      ++currentFrame;
      currentFrame %= animations[anim].duration;
    }
  }

  // @todo Sprite-sprite collision

  bool checkCollision(Vector2 absolutePoint) {
    if(absolutePoint.x < getTransform()->getAbsolutePosition().x || 
       absolutePoint.y < getTransform()->getAbsolutePosition().y) return false;
    Frame& frame = getFrame();
    if(absolutePoint.x >= getTransform()->getAbsolutePosition().x + frame.width || 
       absolutePoint.y >= getTransform()->getAbsolutePosition().y + frame.height) return false;
    return !isspace(frame.collmap[absolutePoint.y - getTransform()->getAbsolutePosition().y][absolutePoint.x - getTransform()->getAbsolutePosition().x]);
  }

  Rect getBoundingRect() {
    Frame& frame = getFrame();
    return {
      getTransform()->getAbsolutePosition(),
      getTransform()->getAbsolutePosition() + Vector2(frame.width, frame.height)
    };
  }

  Frame& getFrame() {
    return animations[anim].frames[currentFrame];
  }

  bool checkCollision(Sprite *sprite) {
    Frame& frame = getFrame();
    Frame& otherFrame = sprite->getFrame();

    Vector2 offset = {
      getTransform()->x() - sprite->getTransform()->x(),
      getTransform()->y() - sprite->getTransform()->y(),
    };

    for(unsigned y = 0; y < frame.height; ++y) {
      for(unsigned x = 0; x < frame.width; ++x) {
        if(!isspace(frame.collmap[y][x]) &&
           offset.x + x > 0 &&
           offset.y + y > 0 &&
           offset.x + x < otherFrame.width &&
           offset.y + y < otherFrame.height &&
           !isspace(otherFrame.collmap[offset.y + y][offset.x + x]))
            return true;
      }
    }
    return false;
  }

};