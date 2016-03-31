#include <iostream>
#include <vector>
#include <memory>
 
struct Statement {
  virtual void Analyze() const = 0;
  
  virtual ~Statement() = default;
};
 
struct YetAnotherStatement: Statement {
  void Analyze() const final {
    std::cout << t << std::endl;
  }
  YetAnotherStatement( int i ): t{ ( double ) i * ( 10.6 / 0.7 ) } {}
private:  
  double t;
};
 
struct OtherStatement: Statement {
  void Analyze() const final {
    std::cout << t << std::endl;
  }
  OtherStatement( int i ): t{ i } {}
private:  
  int t;
};
 
struct CompoundStatement: Statement {
  void Analyze() const final {
    for ( auto&& b : _statements ) {
      b->Analyze();
    }
  }
  
  void Append(std::unique_ptr<Statement const> statement) {
    _statements.push_back(std::move(statement));
  }

private:
  std::vector<std::unique_ptr<Statement const>> _statements;
};
 
struct Declaration {
  Declaration( Statement const * const s ): s_( s ){}
  void Analyze() {
    s_->Analyze();
  }
private:  
  Statement const * const s_;
};
 
int main() {
  auto s = std::make_unique<CompoundStatement>();
  for ( int i = 1; i <= 10; ++i ) {
    if( i % 2 == 0 ) {
      s->Append( std::make_unique<OtherStatement>( i ) );
    } else {
      s->Append( std::make_unique<YetAnotherStatement>( i ) );
    }
  }
  Statement const * const p_s = s.get();
  Declaration d( p_s );
  d.Analyze();

  return 0;
}