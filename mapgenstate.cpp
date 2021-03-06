/*  Copyright (c) 2013, Abdullah A. Hassan <voodooattack@hotmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 *  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "mapgenstate.hpp"
#include "player.hpp"
#include "engine.hpp"
#include "map.hpp"
#include "game.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "mapgenerator.hpp"

#include <physfs.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <stdio.h>

namespace ADWIF
{
  MapGenState::MapGenState(const std::shared_ptr<ADWIF::Engine> & engine, std::shared_ptr<ADWIF::Game> & game):
    myEngine(engine), myGame(game), myViewOffX(0), myViewOffY(0), myViewOffZ(0)
  {
    myEngine->delay(30);
    myEngine->input()->setTimeout(-1);
  }

  MapGenState::~MapGenState() {
    myEngine->delay(50); myEngine->input()->setTimeout(0);
  }

  void MapGenState::init()
  {
    myGame->createMap();

    myViewOffX = 171 * myGame->generator()->chunkSizeX();
    myViewOffY = 169 * myGame->generator()->chunkSizeY();

    myGame->generator()->generateOne(myViewOffX / myGame->generator()->chunkSizeX(),
                                      myViewOffY / myGame->generator()->chunkSizeY(),
                                      myViewOffZ / myGame->generator()->chunkSizeZ());
  }

  void MapGenState::step()
  {
    myEngine->renderer()->clear();
    myEngine->renderer()->drawRegion(myViewOffX, myViewOffY, myViewOffZ, myEngine->renderer()->width(),
                                     myEngine->renderer()->height(), 0, 0, myGame.get(), myGame->map().get());
    myEngine->renderer()->style(White, Black, Style::Bold);
    myEngine->renderer()->drawChar(myEngine->renderer()->width() / 2, myEngine->renderer()->height() / 2, '@');
    std::string str = "Position: " + boost::lexical_cast<std::string>(myViewOffX) +
      "x" + boost::lexical_cast<std::string>(myViewOffY) +
      "x" + boost::lexical_cast<std::string>(myViewOffZ);
    myEngine->renderer()->style(White, Black, Style::Bold);
    myEngine->renderer()->drawText(1,1, str + std::string(myEngine->renderer()->width() - 2 - str.size(),  ' '));
  }

  void MapGenState::consume(int key)
  {
    if (key == Key::Escape)
    {
      myGame->saveMap();
      done(true);
    }
    else if (key == Key::Up)
    {
      myViewOffY--;
    }
    else if (key == Key::Down)
    {
      myViewOffY++;
    }
    else if (key == Key::Left)
    {
      myViewOffX--;
    }
    else if (key == Key::Right)
    {
      myViewOffX++;
    }
    else if (key == '>')
    {
      myViewOffZ--;
    }
    else if (key == '<')
    {
      myViewOffZ++;
    }
    else if (key == 'c')
      myGame->createMap();
    else if (key == 'l')
      myGame->loadMap();
    else if (key == 's')
      myGame->saveMap();
    else if (key == 'g')
    {
      myEngine->input()->setTimeout(0);
      for(unsigned int y = 0; y < myGame->generator()->height(); y++)
        for(unsigned int x = 0; x < myGame->generator()->width(); x++)
          for(int z = -myGame->generator()->depth() / 2; z < myGame->generator()->depth() / 2; z++)
          {
            std::stringstream ss;
            ss << x << "x" << y << "x" << z;
            std::string str = "Generating " + ss.str();
            myEngine->renderer()->startWindow(1,1,myEngine->renderer()->width() - 2, 1);
            myEngine->renderer()->style(White, Black, Style::Bold);
            myEngine->renderer()->drawText(0,0, str + std::string(myEngine->renderer()->width() - 2 - str.size(),  ' '));
            myEngine->renderer()->refresh();
            myEngine->renderer()->endWindow();
            myGame->generator()->generateOne(x, y, z);
            if (myEngine->input()->key() == Key::Escape)
            {
              myEngine->input()->setTimeout(-1);
              return;
            }
            //myEngine->sleep(0);
          }
    }
    myGame->generator()->generateAround(myViewOffX + myEngine->renderer()->width() / 2,
                                        myViewOffY + myEngine->renderer()->height() / 2,
                                        myViewOffZ + (int)myGame->generator()->chunkSizeZ() / 2);
  }

  void MapGenState::activate() { }

}

