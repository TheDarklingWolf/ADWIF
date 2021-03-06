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

#include <signal.h>
//#include <stdio.h>
#include <physfs.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "engine.hpp"
#include "curses.hpp"
#include "fileutils.hpp"

#include "map.hpp"
#include "introstate.hpp"
#include "mapeditorstate.hpp"

#include "util.hpp"

namespace ADWIF
{
  boost::program_options::variables_map options;
}

using namespace ADWIF;
namespace po = boost::program_options;

/* void sig_handler(int code)
{

} */

int main(int argc, char ** argv)
{
  //signal(SIGINT, sig_handler);      /* arrange interrupts to terminate */

  std::ios_base::sync_with_stdio (false);

  setlocale(LC_CTYPE, "");

  PhysFS::init(argv[0]);

  po::options_description odesc("Usage");

  odesc.add_options()
    ("help", "show this help message")
    ("editor", "start in map editor mode")
    ("unicode", "start in Unicode mode (only use this if your terminal and font support it)");

  po::store(po::parse_command_line(argc, argv, odesc), options);
  po::notify(options);

  if (options.count("help")) {
    std::cout << odesc << "\n";
    return 1;
  }

  dirSep = PhysFS::getDirSeparator();
  writeDir = PhysFS::getUserDir() + dirSep + ".adwif";
  dataDir = PhysFS::getBaseDir() + dirSep + "data";
  dataFile = PhysFS::getBaseDir() + dirSep + "data.dat";
  saveDir = writeDir + dirSep + "save";

  boost::filesystem::create_directory(writeDir);
  boost::filesystem::create_directory(saveDir);

  PhysFS::setWriteDir(writeDir);
  PhysFS::mount(dataDir, "/", false);
  PhysFS::mount(dataFile, "/", true);

  std::shared_ptr<Renderer> renderer;
  std::shared_ptr<Input> input;
  std::shared_ptr<Engine> engine;

  renderer.reset(new CursesRenderer());
  input.reset(new CursesInput(renderer));

  if (!renderer->init())
  {
    std::cerr << "Error initialising the display system." << std::endl;
    return 1;
  }

  if (!input->init())
  {
    std::cerr << "Error initialising the input system." << std::endl;
    return 1;
  }

  engine.reset(new Engine(renderer, input));

  std::shared_ptr<GameState> state;

  if(options.count("editor"))
    state.reset(new MapEditorState(engine));
  else
    state.reset(new IntroState(engine));

  engine->addState(state);

  int ret = engine->start();

  PhysFS::deinit();

  return ret;
}

