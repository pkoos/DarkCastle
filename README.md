# Dark Castle MUD

[Dark Castle](http://www.dcastle.org/) is a [MUD](https://en.wikipedia.org/wiki/MUD) that has been running (almost) continually since the early 90s.  Branched from an early version of [DIKU](https://en.wikipedia.org/wiki/DikuMUD) the code has been rewritten multiple times by many authors over the years.  It has been a source of enjoyment and rivalry for many over the years.  In 2020 it was decided to open-source it for the enjoyment of our players and for some nostalgia for those that return for a visit.

The game is available to login via telnet dcastle.org over ports 23, 6969 or 8080 for the standard no-botting/no-multiplaying server or dcastle.org 6666 for the botting/multiplaying server.


You will need to install Docker to build the development container.

From the base directory, run the following command to build the container: 

```
docker build -t `container_tag` .
```

Once the container is built, start it using the following command: 

```
docker start
```

The following command will get you into the container itself: 

```
docker exec -it 'container_tag' /bin/bash
```

To start the app, from the container go to
```
/srv/dcastle2/git/DarkCastle/src
```

and run the following command: 

```
./dcastle -p6969
```

This will attempt to start up the game using port 6969. THERE WILL BE MANY MISSING FILES AND FOLDERS and I would suggest adding them into the directory structure.

One folder you will need that will not be immediately apparent is a save folder in the base git folder. This is the format

```
/save/A/
```

where A is the first initial of your character's name. You can create one for all letters if you'd like, or just the ones you'll use.

Once the folders and files are created, you should be good to go! Be warned, this is an open-source project with the bare minimums of areas, mobs and objects, and as such there will be crashes when you attempt to go to a location that does not have an exit.

To connect to the game, you can use a client such as Mudlet, or connect directly from terminal. Type this command to connect
```
telnet localhost 6969
```