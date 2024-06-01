#ifndef H_SMTBL
#define H_SMTBL

enum class node_type: unsigned char
{
  NUMBER = 0,
  LITERAL,
  IDENTIFIER
};

class symbol_table
{
private:
  class node
  {
  public:
    char *m_name;
    int m_size;
    int m_line_of_declaration;
    int m_address;
    node_type m_type;
    node();
    ~node();
    node(node &) = delete;
    const char *get_name();
    void set_name(const char *name);
    void set_size(int);
    int get_size();
    void set_line(int);
    int get_line();
    void set_address(int);
    int get_address();
    void set_type(node_type);
    node_type get_type();
  };
  node* table[100]; // Array for lexemes, keyword HASH to find all depends parts of codes
public:
  symbol_table();
  ~symbol_table();
  node* get_node(int);
  int install_node(const char *, const char *, node_type);
  static unsigned int hash(const char *, const char *);
  void dump_table(void);
};

#endif
