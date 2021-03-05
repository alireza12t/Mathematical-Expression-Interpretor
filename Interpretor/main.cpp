#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <math.h>
#include <string>
#include <vector>
#include <iterator>
#include <stdlib.h>

const int LEFT_ASSOC  = 0;
const int RIGHT_ASSOC = 1;

using namespace::std;

typedef map< string, pair< int,int >> OpMap;
typedef vector<string>::const_iterator cv_iter;
typedef string::iterator s_iter;

const OpMap::value_type assocs[] =
{
    OpMap::value_type( "+", make_pair( 0, LEFT_ASSOC ) ),
    OpMap::value_type( "-", make_pair( 0, LEFT_ASSOC ) ),
    OpMap::value_type( "*", make_pair( 5, LEFT_ASSOC ) ),
    OpMap::value_type( "%", make_pair( 5, LEFT_ASSOC ) ),
    OpMap::value_type( "^", make_pair( 5, LEFT_ASSOC ) ),
    OpMap::value_type( "/", make_pair( 5, RIGHT_ASSOC ) )
};

const OpMap opmap( assocs, assocs + sizeof( assocs ) / sizeof( assocs[ 0 ] ) );

bool isParenthesis( const string& token) {
    return token == "(" || token == ")";
}

bool isOperator( const string& token) {
    return token == "+" || token == "-" ||
    token == "*" || token == "/" || token == "%" || token == "^";
}

bool isAssociative( const string& token, const int& type) {
    const pair<int,int> p = opmap.find( token )->second;
    return p.second == type;
}

int cmpPrecedence( const string& token1, const string& token2 ) {
    const pair<int,int> p1 = opmap.find( token1 )->second;
    const pair<int,int> p2 = opmap.find( token2 )->second;
    
    return p1.first - p2.first;
}

bool isEquationParantesesValid( const vector<string>& inputTokens, const int& size, vector<string>& strArray ) {
    bool success = true;
    
    list<string> out;
    vector<string> stack;
    for ( int i = 0; i < size; i++ ) {
        const string token = inputTokens[ i ];
        
        if ( isOperator( token ) ) {
            const string o1 = token;
            
            if ( !stack.empty() )
            {
                string o2 = stack[stack.size()-1];
                
                while ( isOperator( o2 )
                       && ( ( isAssociative( o1, LEFT_ASSOC ) &&  cmpPrecedence( o1, o2 ) == 0 ) ||
                        ( cmpPrecedence( o1, o2 ) < 0 ) )
                       )
                {
                    stack.pop_back();
                    out.push_back( o2 );
                    
                    if ( !stack.empty() )
                        o2 = stack[stack.size()-1];
                    else
                        break;
                }
            }
            
            stack.push_back(o1);
        }
        else if ( token == "(" ) {
            stack.push_back( token );
        }
        else if ( token == ")" ) {
            string topToken  = stack[stack.size()-1];
            
            while ( topToken != "(" ) {
                out.push_back(topToken );
                stack.pop_back();
                
                if ( stack.empty() ) break;
                topToken = stack[stack.size()-1];
            }
            
            if ( !stack.empty() ) stack.pop_back();

            if ( topToken != "(" ) {
                return false;
            }
        }
        else {
            out.push_back( token );
        }
    }
    
    while ( !stack.empty() ) {
        const string stackToken = stack[stack.size()-1];
        
        if ( isParenthesis( stackToken ) ) {
            return false;
        }
        
        out.push_back( stackToken );
        stack.pop_back();
    }
    
    strArray.assign( out.begin(), out.end() );
    
    return success;
}


double calculate( vector<string> tokens ) {
    vector<string> st;
    
    for ( int i = 0; i < (int) tokens.size(); ++i )
    {
        const string token = tokens[ i ];
        
        if ( !isOperator(token) ) {
            st.push_back(token);
        }
        else {
            double result =  0.0;
            
            const string val2 = st[st.size()-1];
            st.pop_back();
            const double d2 = strtod( val2.c_str(), NULL );
            
            if ( !st.empty() ) {
                const string val1 = st[st.size()-1];
                st.pop_back();
                const double d1 = strtod( val1.c_str(), NULL );
                
                if (token == "+") {
                    result = d1 + d2;
                } else if (token == "-") {
                    result = d1 - d2;
                } else if (token == "^") {
                    result = pow(d1, d2);
                } else if (token == "%") {
                    result = fmod(d1, d2);
                } else if (token == "*") {
                    result = d1 * d2;
                } else {
                    result = d1 / d2;
                }
            }
            else {
                if ( token == "-" )
                    result = d2 * -1;
                else
                    result = d2;
            }
            
            ostringstream s;
            s << result;
            st.push_back(s.str());
        }
    }
    
    return strtod( st[st.size()-1].c_str(), NULL );
}

vector<string> tokenize( const string& expression ) {
    vector<string> tokens;
    string str = "";
    
    for ( int i = 0; i < (int) expression.length(); ++i ) {
        const string token( 1, expression[ i ] );
        
        if ( isOperator( token ) || isParenthesis( token ) ) {
            if ( !str.empty() ) {
                tokens.push_back( str ) ;
            }
            str = "";
            tokens.push_back( token );
        }
        else {
            if ( !token.empty() && token != " " ) {
                str.append( token );
            }
            else {
                if ( str != "" ) {
                    tokens.push_back( str );
                    str = "";
                }
            }
        }

    }
    
        if ( str != "" ) {
    tokens.push_back( str );
        }
    return tokens;
}

int main() {
    
    cout << "Input expression: ";
    string input = "";
    cin >> input;
    
    vector<string> tokens = tokenize( input );
    
    vector<string> rpn;
    if ( isEquationParantesesValid( tokens, tokens.size(), rpn ) ) {
        double d = calculate( rpn );
        cout << "Result = " << d << endl;
    }
    else
    {
        cout << "Mis-match in parentheses" << endl;
        return 1;
    }
    
    return 0;
}
