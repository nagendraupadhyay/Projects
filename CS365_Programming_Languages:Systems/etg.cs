using System;
using System.IO;
using System.Collections.Generic;

struct Coords {
    public int x;
    public int y;

    public Coords(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public override bool Equals(Object obj) {
        return obj is Coords c && this == c;
    }

    public override int GetHashCode() {
        return this.x.GetHashCode() ^ this.y.GetHashCode();
    }

    public static bool operator ==(Coords a, Coords b) {
        return a.x == b.x && a.y == b.y;
    }

    public static bool operator !=(Coords a, Coords b) {
        return !(a == b);
    }
}

class key:Entity{
    public override  void look() {
        Console.WriteLine($"You see a key on the ground! Might need that to get out of here...");
    }
    public override  void interact(Player player){
        Console.WriteLine($"You picked up a key!");
        player.hasKey = 1;
    }
}

class loot:Entity{
    public int CoinCounter{
        get;
        set;
    }
    public override  void look() {
        Console.WriteLine($"You see what looks like the corner of a treasure chest poking out of the ground.");
    }
    public override void interact(Player player){
        Console.WriteLine($"You open the chest and find " + CoinCounter + " coins!");
        player.CoinC += CoinCounter;
    }
}

class skeleton:Entity{
    public override  void look() {
        Console.WriteLine($"Not much to see here.");
    }
    public override void interact(Player player){
        Console.WriteLine($"A bony arm juts out of the ground and grabs your ankle!");
        Console.WriteLine($"You've been dragged six feet under by a skeleton.");
        player.dead = 1;
    }
}

class Level {
    private Location[,] grid;
    private int width;
    private int height;

    
    public void set_size(int x, int y){
        this.width = x;
        this.height = y;
        this.grid = new Location[this.width, this.height];
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                this.grid[i,j] = new Location();
            }
        }
    }

    public Level() {
        
    }

    /*public set_exit(){
        return
    }*/

    public Location get_location(int x, int y) {
        return this.grid[x, y];
    }

    public int get_width() {
        return this.width;
    }

    public int get_height() {
        return this.height;
    }
    public void addtolocation(int x, int y, Location curr){
        this.grid[x,y] = curr;
    }
}

class exit:Location{
    //Console.WriteLine($"That looks like the gate out of this spooky place!");
    public void lookatexit(Player player){
        if(player.hasKey == 1){
           Console.WriteLine($"You open the gate with your key!");
        }else{
            Console.WriteLine($"The gate is locked");
        }
    }
}

class Location {
    private Entity[] entities;
    public List<key> KeyList{
        get; 
        set;
    }
    public List<loot> LootList{
        get;
        set;
    }
    public List<skeleton> SkeletonList{
        get;
        set;
    }
    public Location() {
        this.entities = new Entity[0];
        KeyList = new List<key>();
        LootList = new List<loot>();
        SkeletonList = new List<skeleton>();
    }
    public void add_key(){
        KeyList.Add(new key());
    }
    public void add_loot(int count){
        loot L = new loot();
        L.CoinCounter = count;
        LootList.Add(L);
    }
    public void add_skeleton(){
        SkeletonList.Add(new skeleton());
    }
    public void add_entity(Entity entity) {
        Array.Resize(ref this.entities, this.entities.Length + 1);
        this.entities[this.entities.Length - 1] = entity;
    }
    public void remove_entity(Entity entity) {
        int index = Array.IndexOf(this.entities, entity);
        if (index != -1) {
            for (int i = index; i < this.entities.Length - 1; i++) {
                this.entities[i] = this.entities[i + 1];
            }
            Array.Resize(ref this.entities, this.entities.Length - 1);
        }
    }

    public bool has_entity(Type entity_type) {
        foreach (Entity entity in this.entities) {
            if (entity.GetType() == entity_type) {
                return true;
            }
        }
        return false;
    }

    public void look() {
        Console.WriteLine("You see:");
        foreach (Entity entity in this.entities) {
            entity.look();
        }
    }

    public void interact(Player player) {
        foreach (Entity entity in this.entities) {
            entity.interact(player);
        }
    }
}

abstract class Entity {
    public virtual  void look() {}
    public abstract void interact(Player player);
}

class Player {
    public Coords coords { get; set; }

    public bool alive;

    public int CoinC{
        get;
        set;
    }

    public int hasKey{
        get;
        set;
    }

    public int dead{
        get;
        set;
    }

    public Player() {
        this.coords = new Coords(0, 0);
        CoinC = 0;
        hasKey = 0;
        dead = 0;
    }

    public bool is_at(Coords xy) {
        return this.coords == xy;
    }

    public bool is_alive() { return alive; }

    public bool has_key() { 
        if(hasKey == 1){return true;}
        else{return false;}
    }

    public void print_stats() {
        Console.WriteLine($"  LOCATION: {this.coords.x}, {this.coords.y}");
        Console.WriteLine($"  COINS:    {this.CoinC}");
        if(hasKey == 0){
            Console.WriteLine($"  KEY:     False");
        }else{
            Console.WriteLine($"  KEY:      True");
        }

        if(dead == 0){
            Console.WriteLine($"  DEAD:     False");
        }else{
            Console.WriteLine($"  DEAD:      True");
        }
    }
}

class Game {
    int    num_turns;
    Level  level;
    public Player player { get; }

    public Game() {
        this.player = new Player();
        this.player.alive = true;
    }

    public void load(string path) {
        this.level = new Level();

        string line;
        using (StreamReader reader = new StreamReader(path)) {
            while ((line = reader.ReadLine()) != null) {
                if (line == "") { continue; }

                string[] split = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);

                if (split.Length < 3) {
                    Console.WriteLine($"Bad command in level file: '{line}'");
                    Environment.Exit(1);
                }

                int x     = int.Parse(split[1]);
                int y     = int.Parse(split[2]);
                int count = 0;

                if (split.Length > 3) {
                    count = int.Parse(split[3]);
                }

                switch (split[0]) {
                    case "size":
                        // Set the level's size to x by y
                        this.level.set_size(x, y);
                        break;
                    case "exit":
                        // Set the level's exit location to be x, y
                        Location exit = new exit();
                        this.level.addtolocation(x,y,exit);
                        break;
                    case "key":
                        // Add a key to location x, y
                        this.level.get_location(x,y).add_key();
                        break;
                    case "loot":
                        // Add loot to location x, y with count coins
                        this.level.get_location(x,y).add_loot(count);
                        break;
                    case "skeleton":
                        // Add a skeleton to location x, y
                        this.level.get_location(x,y).add_skeleton();
                        break;
                    default:
                        Console.WriteLine($"Bad command in level file: '{line}'");
                        Environment.Exit(1);
                        break;

                }
            }
        }
    }

    public void input(string line) {
        this.num_turns += 1;

        if(num_turns > 2 * level.get_height() * level.get_width()){
            this.player.alive = false;
            Console.WriteLine("You have died from exhaustion.");
            this.exit();
        }

        Console.WriteLine("================================================================");

        string[] split = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);

        if (split.Length != 2) {
            Console.WriteLine($"Bad command in input: '{line}'");
            return;
        }

        Coords new_coords = this.player.coords;
        switch (split[1]) {
            case "north":
                new_coords.y += 1;
                break;
            case "south":
                new_coords.y -= 1;
                break;
            case "east":
                new_coords.x += 1;
                break;
            case "west":
                new_coords.x -= 1;
                break;
            default:
                Console.WriteLine($"Bad command in input: '{line}'");
                return;
        }

        // Are the new coords valid?
        switch (split[0]) {
            case "go":
                if(new_coords.x < 0 || new_coords.y < 0 || new_coords.x >= this.level.get_width() || new_coords.y >= this.level.get_height()){
                    Console.WriteLine($"A towering wall is before you. This must be the edge of the graveyard.");
                }else{
                    if(this.level.get_location(new_coords.x, new_coords.y) is exit){
                        Console.WriteLine($"That looks like the gate out of this spooky place!");
                        Location loc = this.level.get_location(new_coords.x, new_coords.y);
                        if (loc is exit) {
                            exit curr = (exit) loc;
                            curr.lookatexit(this.player);
                            //exit();
                        }
                    }

                    this.player.coords = new_coords;

                    foreach(key k in this.level.get_location(new_coords.x,new_coords.y).KeyList){
                        k.look();
                    }
                    foreach(loot k in this.level.get_location(new_coords.x,new_coords.y).LootList){
                        k.look();
                    }
                    foreach(skeleton k in this.level.get_location(new_coords.x,new_coords.y).SkeletonList){
                        k.look();
                    }

                    foreach(key k in this.level.get_location(this.player.coords.x, this.player.coords.y).KeyList){
                        k.interact(this.player);
                    }
                    foreach(loot k in this.level.get_location(this.player.coords.x,this.player.coords.y).LootList){
                        k.interact(this.player);
                    }
                    foreach(skeleton k in this.level.get_location(this.player.coords.x,this.player.coords.y).SkeletonList)
                    {
                        k.interact(this.player);
                    }
                }
                break;
            case "look":
                if(new_coords.x < 0 || new_coords.y < 0 || new_coords.x >= this.level.get_width() || new_coords.y >= this.level.get_height()){
                    Console.WriteLine($"A towering wall is before you. This must be the edge of the graveyard.");
                }else{
                    if(this.level.get_location(new_coords.x, new_coords.y) is exit){
                        Console.WriteLine($"That looks like the gate out of this spooky place!");
                    }

                    foreach(key k in this.level.get_location(new_coords.x,new_coords.y).KeyList){
                        k.look();
                    }
                    foreach(loot k in this.level.get_location(new_coords.x,new_coords.y).LootList){
                        k.look();
                    }
                    foreach(skeleton k in this.level.get_location(new_coords.x,new_coords.y).SkeletonList){
                        k.look();
                    }
                }
                break;
                
            default:
                Console.WriteLine($"Bad command in input: '{line}'");
                return;
        }
    }

    bool is_over() {
        // What are the exit conditions?
        //return this.level.is_exit(this.player.coords) || !this.player.is_alive();
        //return false;
        if(this.num_turns > (2 * this.level.get_height() * this.level.get_width())){
            return true;
        }else{
            return false;
        }
    }

    void print_stats() {
        if (!this.is_over() && this.player.is_alive()) {
            Console.WriteLine("You successfully escaped the graveyard!");
        } else {
            Console.WriteLine("You did not escape the graveyard. GAME OVER");
        }
        Console.WriteLine($"Game ended after {this.num_turns} turn(s).");
        player.print_stats();
    }

    public void exit() {
        Console.WriteLine("================================================================");
        this.print_stats();
        Environment.Exit(0);
    }

    public void exit_if_over() {
        if (this.is_over()) { this.exit(); }
    }

    public void intro() {
        Console.WriteLine("You awake in a daze to find yourself alone in the dead of night, surrounded by headstones...");
        Console.WriteLine("You must escape this graveyard.");
        Console.WriteLine("================================================================");
        // Look at the current location.
        Console.WriteLine($"Not much to see here.");
        Console.Write($"{this.player.coords.x}, {this.player.coords.y}> ");
    }
}

class ETG {
    static void Main(string[] args) {
        if (args.Length != 1) {
            Console.WriteLine("ERROR: expected a single argument (the level file)");
            Environment.Exit(1);
        }

        Game game = new Game();

        game.load(args[0]);
        game.intro();

        game.exit_if_over();

        string line;

        while ((line = Console.ReadLine()) != null) {
            if (line == "") { continue; }
            game.input(line);
            game.exit_if_over();
            Console.Write($"{game.player.coords.x}, {game.player.coords.y}> ");
        }

        game.exit();
    }
}
