vector<Vertex> clippingBoundaries; //global
//Change DefineTreeLines--> now has paramter of which vector to put it in
void defineClipBounds(vector<Vertex> cb)
{
   defineTreeLines(VIEWPORT_MIN, VIEWPORT_MIN, VIEWPORT_MIN, VIEWPORT_MAX, cb);
   defineTreeLines(VIEWPORT_MIN, VIEWPORT_MIN, VIEWPORT_MAX, VIEWPORT_MIN, cb);
   defineTreeLines(VIEWPORT_MAX, VIEWPORT_MIN, VIEWPORT_MAX, VIEWPORT_MAX, cb);
   defineTreeLines(VIEWPORT_MIN, VIEWPORT_MAX, VIEWPORT_MAX, VIEWPORT_MAX, cb);
}

void intersect(Vertex first, Vertex second, Vertex clipBound1, Vertex clipBound2, Vertex pointIntersect)
{
  if(clipBound1.y == clipBound2.y)
  {
    pointIntersect.y = clipBound1.y;
    pointIntersect.x = (first.x + (clipBound1.y - first.y)) * ((second.x - first.x)/(second.y - first.y));
  }
  
  else
  {
    pointIntersect.x = clipBound1.x;
    pointIntersect.y = (first.y + (clipBound1.x - first.x)) * ((second.y - first.y)/(second.x - first.x));
  }
}


bool insideBound(Vertex vert, Vertex clipBound1, clipBound2)
{
  if(clipBound2.x > clipBound1.x){
    if(vert.y > = clipBound1.y)  
      return true;
  }
  
  if(clipBound2.x < clipBound1.x){
    if(vert.y < = clipBound1.y)
      return true;
  }
  
  if(clipBound2.y > clipBound1.y){
    if(vert.x < = clipBound1.x)
      return true;
  }
  
  if(clipBound2.y < clipBound1.y){
    if(vert.x > = clipBound1.x)
      return true;
  }
  
  return false;
}

void output(Vertex newVert, int outputLength, vector<Vertex> outputVec)
{
  //don't use this--need to push to output vector
  output.push_back(newVert);
}
  


void SHPolygonClip(vector<Vertex> inputVec, vector<Vertex> outputVec, Vertex clipBound1, Vertex clipBound2)
{
  Vertex a,b,c; 
  
  outputLength = 0;
  a = inputVec.back();
  for(int i = 0; i < inputVec.size(); i++)
  {
    b = inputVec[i];
    
    if(insideBound(b, clipBound1, clipBound2)
      if(insideBound(a, clipBound1, clipBound2)
        outputVec.push_back(b);
      else{
        intersect(a, b, clipBound1, clipBound2, c)
        outputVec.push_back(c);
        outputVec.push_back(b);
      }
    else if(nsideBound(a, clipBound1, clipBound2)
    {
      intersect(a,b, clipBound1, clipBound2, c);
      outVec.push_back(c);
    }
    
    a=b;
  } //end for
}

void processPolyClip()
{
  vector<Vertex> clippingBounds;
  defineClipBounds(clippingBounds);
  
  vector<Vertex> outputCirc;
  vector<Vertex> outputBase;
  
  for(int i = 0; i < clippingBounds.size(); i+2)
  {
    SHPolygonClip(transformationCircle.myCircPoints, outputCirc, clippingBounds[i], clippingBounds[i+1]);
    SHPolygonClip(transformationBaseTree.myBasePoints, outputBase, clippingBounds[i], clippingBounds[i+1]);
  }
  
}
