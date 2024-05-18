#ifndef H_SMTBL
#define H_SMTBL

class symbol_table
{
  class node
  {
  public:
    char *name;
    int size;
    int line_of_declaration;
    int address;

    node();
    ~node();
    node(node &);
    const char *get_name();
    void set_name(const char *name);
    void set_size(int);
    int get_size();
    void set_line(int);
    int get_line();
    void set_address(int);
    int get_address();
  };
  node* table[100]; // Array for lexemes, keyword HASH to find all depends parts of codes
public:
  symbol_table();
  ~symbol_table();
  node* get_node(int);
  int install_id(const char *, const char *);
  static unsigned int hash(const char *, const char *);
  void dump_table(void);
};

#endif
