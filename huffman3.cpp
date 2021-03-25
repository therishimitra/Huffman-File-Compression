// FILE: huffman3.cpp
// This file implements Huffman coding and decoding.
#include <limits>
#include <bitset>
#include <fstream>
#include <list>
#include <string>
#include <cassert>
#include "node.h"

#define ASCII_TABLE_LENGTH 128

double probabilityChart[ASCII_TABLE_LENGTH] = {0}; 		// Indexes 0 to 127 cover all relevant ASCII characters
string Hcode[ASCII_TABLE_LENGTH];

//Helper functions

void traverse(node *root, string code);
void frequencyReader();
void finalEncoder(string encodedString);
void finalDecoder(node *root);


int main()
{

	
	frequencyReader();
	
	cout<<"\nCreating node list:\n";
//////////////////////   Creating Node List   /////////////////////	
	list<node*> node_list;
	for (unsigned int n=0; n<ASCII_TABLE_LENGTH; n++)
	{
		if(probabilityChart[n]!=0)
		{
			node* newnode = new node((char)n, probabilityChart[n]);
			node_list.push_back(newnode);
			cout << "Pushed to list: " << n <<" "<< probabilityChart[n] <<endl;		
		}
	}
	
	cout<<"List of nodes has been created\n\n";

///////////////////   Construct Huffman Coding Tree ////////////////////

	while (node_list.size() > 1) 
	{		
		// Implementation from part 1: 
		
		node_list.sort(comp_prob);
		
		node* firstLowestProb = node_list.front();
		node_list.pop_front();
		
		node* secondLowestProb = node_list.front();
		node_list.pop_front();

		double p = (firstLowestProb->probability)+(secondLowestProb->probability);

		node* newnode = new node('\0', p);
		node_list.push_back(newnode);

		newnode->left=firstLowestProb;
		newnode->right=secondLowestProb;
		
		node_list.sort(comp_prob);
		
	}
	cout<<"Huffman coding tree contructed\n\n";
///////////////////   Generate Huffman Codes ////////////////////////

	//-----------------------------------------------
	// Implementation from part 1: 
	cout<<"Traversing Huffman coding tree...\n\n";

	for(int i=0; i<ASCII_TABLE_LENGTH; i++)
	{
		string code="";
		traverse(node_list.front(), code);
	}
	
	///////////Printing Huffman codes///////////////////////////////
	cout<<"\nHuffman codes:"<<endl;
	for(int i=0; i<ASCII_TABLE_LENGTH; i++)
	{
		cout<<i<<" ";
		cout<<Hcode[i]<<endl;
	}	
		
	
/////////////////////////   Encode Input File   ////////////////////////////////

	// Open the text file for reading
	string textfilename = "input.txt";
	ifstream textfile;
	textfile.open(textfilename.c_str(), ifstream::in);

	assert(textfile);
	
	textfile.clear();
	textfile.seekg(0, ios::beg);

	cout<<"\nSuccessfully opened input text file: "<<textfilename<<endl;


	string encodedString="";

	
	// Input and encode each character from the input file one by one
	// and output them to the output string
	char c;
	while (textfile.get(c))
	{
		
		// Fetch the code
		string s = Hcode[(int)c];

		//Write code to encoded string	
		if (!s.empty())
		{
			encodedString+=s;
		}	

		else 
		{
			cout << "Error: cannot find the code for " << c << endl;
			//return 0;
		}		
	}
	

	// Close the text file
	textfile.close();
	
	// Call finalEncoder()

	//cout<<"Encoded string:\n"<<encodedString<<endl;
	finalEncoder(encodedString);
//////////////////////// Decode the Encoded File ///////////////////////////
	finalDecoder(node_list.front());
    
	clean(node_list.front());
	
	return 0;
}

//Calculates and creates a file with probabilities and stores them in an array
void frequencyReader()
{
	
	int total_character_count=0;
	
	string textfilename = "input.txt";
	ifstream textfile;
	textfile.open(textfilename.c_str(), ifstream::in);
	assert(textfile);
	
	char c;
	////////Counting characters//////////////////
	while (textfile.get(c))
	{
			// Read the characters
			
			//textfile.get(c);

			total_character_count++;
			probabilityChart[(int)c]++;
			cout<<c;			
	}
	textfile.close();

	////////////Creating probability.txt///////////////
	string probabilityfilename = "probability.txt";
	ofstream probabilityfile;
	probabilityfile.open(probabilityfilename.c_str(), ofstream::out);
	assert(probabilityfile);
	cout<<"Creating probability.txt:\n\n";
	for(int i=0; i<ASCII_TABLE_LENGTH; i++)
	{
		//char ch = i+32;
		probabilityChart[i]/=total_character_count;
		if (probabilityfile.is_open())
		{
			cout <<i<<"\t"<<probabilityChart[i]<<"\n";
			probabilityfile <<i<<"\t"<<probabilityChart[i]<<"\n";
						
		}
			
		else
		{
			cout<<"Error: output file cannot be opened!\n";
			return;
		}
	}
	probabilityfile.close();
	
}

//Traverses Huffman coding tree
void traverse(node *root, string code)
{
	
	if (root == nullptr)
        return;
 
    // found a leaf node
    if (!root->left && !root->right) 
	{
		Hcode[(int)(root->letter)]=code;
        
    }
 
    traverse(root->left, code + "0");
    traverse(root->right, code + "1");    
	
}

//Creates binary encoded file
void finalEncoder(string encodedString)
{

	//cout<<encodedString;
	
	ofstream encoder;
	encoder.open("finalEncoded.txt", ios::out | ios::binary);	

	assert(encoder);

	string byteString="";
	int count=0;
	
	while(count < encodedString.length())
	{
		char c =0x00;
		char* ptr = &c;

		for(int i = count; i < count+8; i++)
		{
			byteString+=encodedString[i];
		}
		//cout<<byteString<<endl;
		
		for (int j = 0; j < byteString.length(); j++)
		{
 			if (byteString[j]=='1') c = (c<<1) | 0x01; 	// append 1 in the end
 			if (byteString[j]=='0') c = (c<<1) | 0x00; 	// append 0 in the end
		}

		encoder.write(ptr,1);


		byteString="";
		count+=8;
	}
	encoder.close();	
}

// http://en.cppreference.com/w/cpp/language/type_alias
using bite = unsigned char ;

// http://en.cppreference.com/w/cpp/types/numeric_limits
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<bite>::digits ;

// http://en.cppreference.com/w/cpp/utility/bitset
using bits_in_byte = std::bitset<BITS_PER_BYTE> ;


void finalDecoder(node *ptr)
{
	// Open the file created by finalEncoder()
	ifstream encoded;
	encoded.open("finalEncoded.txt", ios::in | ios::binary);	
	assert(encoded);

	// Open the file for writing decoded text
	string decodedfilename = "FinalDecoded.txt";
	ofstream decoder;
	decoder.open(decodedfilename.c_str(), ofstream::out);
	assert(decoder);

	node *front = ptr;

	string s = "";
	char c;
	char bit;
	while (encoded.get(c))
	{
		s+= bits_in_byte( bite(c) ).to_string() ;
	}

	//cout<<"Bit string:\n"<<s<<endl;

	for(int i=0; i<s.length(); i++)
	{
		//Move left if '0'
			if(s[i]=='0')
				ptr=ptr->left;

			//Move right if '1'
			else if(s[i]=='1')
				ptr=ptr->right; 

			// found a leaf node
			if (!ptr->left && !ptr->right) 
			{
				decoder << ptr->letter;	//send letter on leaf node to output file
				ptr = front;    //go back to root		
			}
	}

	encoded.close();
	decoder.close();

}