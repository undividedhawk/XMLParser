#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include "curl/curl.h"
#include <iostream>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <Windows.h>
using namespace rapidxml;
using namespace std;


static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int main()
{
  float currentBid;
  string value  = "stuff";
  string sym;
  float highBid;
  float bidValue;
  cout << "enter the currency you wish to trade in: ";
  cin >> sym;
  cout << endl;
  cout << "enter the minimum bid you are looking for: ";
  cin >> highBid;
  cout << endl;
  
  
  double startTime = GetTickCount();
  while(true){
      double currentTime = GetTickCount() - startTime;
      
      
      //updater
      if(currentTime>=5000)
      {
      
  CURL *curl_handle;
  static const char *pagefilename = "page.xml";
  FILE *pagefile;
  
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* set URL to get here */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, "http://rates.fxcm.com/RatesXML");
 
  /* Switch on full protocol/debug output while testing */ 
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
  /* disable progress meter, set to 0L to enable and disable debug output */ 
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
 
  /* open the file */ 
  pagefile = fopen(pagefilename, "wb");
  if(pagefile) {
 
    /* write the page body to this file handle */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
 
    /* get it! */ 
    curl_easy_perform(curl_handle);
 
    /* close the header file */ 
    fclose(pagefile);
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
  
  
  rapidxml::file<> xmlFile("page.xml");
  rapidxml::xml_document<> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> *node = doc.first_node("Rates");
  xml_node<> *node2 = node->first_node();

  while(node2){
      for(xml_attribute<> *attr = node2->first_attribute(); attr; attr = attr->next_attribute()){
          value = attr->value();      
      }
      if(value == sym){
          xml_node<> *node3 = node2->first_node();
          bidValue = atof(node3->value());
          if(bidValue >= highBid){
              currentBid = atof(node3->value());
              break;
          }
      }
      node2 = node2->next_sibling();
  }
  startTime = GetTickCount();
  }
      if(bidValue >=highBid){
          cout << "high bid found going at a rate of : " << currentBid;
          break;
      }
  }
}