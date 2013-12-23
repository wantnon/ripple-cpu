

#include <iostream>
using namespace std;
#include "RippleModel.h"


using namespace cocos2d ;
RippleModel::RippleModel() 
: isNeedSimulation(true)
{
    
}

RippleModel::~RippleModel() 
{
  	if(rippleCoeff)free(rippleCoeff); 
	if(rippleSource)free(rippleSource);
	if(rippleDest)free(rippleDest);
	if(rippleVertices)free(rippleVertices);
	if(rippleTexCoords)free(rippleTexCoords);
	if(rippleIndicies)free(rippleIndicies);
	if(s_tcBuff)free(s_tcBuff);
	if(t_tcBuff)free(t_tcBuff);
}

void RippleModel::initRippleCoeff() // the rippleCoff is the template of touch hole, a very small mat((2*touchRadius+1)x(2*touchRadius+1))
{
	unsigned _touchRadius = 2*touchRadius;
	float distance = 0.0f;
	float factor = 0.0f;

    for (unsigned y=0; y<=_touchRadius; ++y)
    {
        for (unsigned x=0; x<=_touchRadius; ++x)
        {        
            distance = sqrtf(float((x-touchRadius)*(x-touchRadius)+(y-touchRadius)*(y-touchRadius)));
            
            if (distance <= touchRadius)
            {
                factor = (distance/touchRadius);
                
                // goes from -512 -> 0
                rippleCoeff[y*(touchRadius*2+1)+x] = -(cosf((float)(factor*M_PI))+1.f) * 256.f;
				//there are touchRadius*2+1 points in each row, so [y*(touchRadius)+x] equals to [y][x] 
            }
            else
            {
                rippleCoeff[y*(touchRadius*2+1)+x] = 0.f;   
            }
        }
    }
}

void RippleModel::initMesh()
{
	float maxX=0;
	float maxY=0;
    for (int i=0; i<poolHeight; ++i)
    {
        for (int j=0; j<poolWidth; ++j)
        {
            rippleVertices[(i*poolWidth+j)*2+0] = meshFactor * j;
            rippleVertices[(i*poolWidth+j)*2+1] = meshFactor * i;
			if(rippleVertices[(i*poolWidth+j)*2+0]>maxX)maxX=rippleVertices[(i*poolWidth+j)*2+0];
			if(rippleVertices[(i*poolWidth+j)*2+1]>maxY)maxY=rippleVertices[(i*poolWidth+j)*2+1];

            rippleTexCoords[(i*poolWidth+j)*2+0] = ((float)j/(poolWidth-1)) ;
            rippleTexCoords[(i*poolWidth+j)*2+1] = 1-((float)i/(poolHeight-1)) ;
        }
    }
	cout<<"maxX:"<<maxX<<endl;
	cout<<"maxY:"<<maxY<<endl;
    
    int index = 0;
    for (int i=0; i<poolHeight-1; ++i)
    {
        for (int j=0; j<poolWidth; ++j)
        {
            if (i%2 == 0)
            {
                // emit extra index to create degenerate triangle
                if (j == 0)
                {
                    rippleIndicies[index] = (i*poolWidth+j);
                    ++index;                                                                                                                                                                   
                }
                
                rippleIndicies[index] = (i*poolWidth+j);
                ++index;                                                                                                                                                                                           
                rippleIndicies[index] = ((i+1)*poolWidth+j);
                ++index;
                
                // emit extra index to create degenerate triangle
                if (j == (poolWidth-1))
                {
                    rippleIndicies[index] = ((i+1)*poolWidth+j);
                    ++index;
                }
            }
            else
            {
                // emit extra index to create degenerate triangle
                if (j == 0)
                {
                    rippleIndicies[index] = ((i+1)*poolWidth+j);
                    ++index;
                }
                
                rippleIndicies[index] = ((i+1)*poolWidth+j);
                ++index;
                rippleIndicies[index] = (i*poolWidth+j);
                ++index;
                
                // emit extra index to create degenerate triangle
                if (j == (poolWidth-1))
                {
                    rippleIndicies[index] = (i*poolWidth+j);
                    ++index;
                }
            }
        }
    }

}



bool RippleModel::initWithScreenWidth( int width, 
                          int height,
                          int factor, 
                          int radius,
                          int texWidth,
                          int texHeight) 
{
    _screenWidth = width;
    _screenHeight = height;
    meshFactor = factor;
    poolWidth = texWidth/meshFactor+1;
    poolHeight = texHeight/meshFactor+1;
    touchRadius = radius;
    textureWidth = texWidth ;
    textureHeight = texHeight ;
    CCLOG("meshFactor:%f",(float)meshFactor);
    CCLOG("pool size:%f,%f",(float)poolWidth,(float)poolHeight);

   
    
    rippleCoeff = (float *)calloc((touchRadius*2+1)*(touchRadius*2+1), sizeof(float));
    
    // +2 for padding the border
    rippleSource = (float *)calloc((poolWidth+2)*(poolHeight+2), sizeof(float));

    rippleDest = (float *)calloc((poolWidth+2)*(poolHeight+2), sizeof(float));

    rippleVertices = (float *)calloc(poolWidth*poolHeight*2, sizeof(float));

    rippleTexCoords = (float *)calloc(poolWidth*poolHeight*2, sizeof(float));

    rippleIndicies = (int *)calloc((poolHeight-1)*(poolWidth*2+2), sizeof(int));
    
	//s_tcBuff and t_tcBuff is the inital texCoord, the Descartes product of s_tcBuff and t_tcBuff is texCoord matrix
    s_tcBuff = (float *)calloc(poolWidth, sizeof(float));
	t_tcBuff = (float *)calloc(poolHeight, sizeof(float));


	posArrayLen=poolWidth*poolHeight*2;
	texCoordArrayLen=posArrayLen;
	indexArrayLen=(poolHeight-1)*(poolWidth*2+2);

	for (unsigned int x=0; x<poolWidth; ++x)
	{
		s_tcBuff[x] = (float)x/(poolWidth-1);
	}

	for (unsigned int y = 0; y < poolHeight; ++y)
	{
		t_tcBuff[y] =1- ((float)y/(poolHeight-1)); 
	}

    if (!rippleCoeff || !rippleSource || !rippleDest || 
        !rippleVertices || !rippleTexCoords || !rippleIndicies||
		!s_tcBuff||!t_tcBuff)
    {
		if(rippleCoeff)free(rippleCoeff); 
		if(rippleSource)free(rippleSource);
		if(rippleDest)free(rippleDest);
		if(rippleVertices)free(rippleVertices);
		if(rippleTexCoords)free(rippleTexCoords);
		if(rippleIndicies)free(rippleIndicies);
		if(s_tcBuff)free(s_tcBuff);
		if(t_tcBuff)free(t_tcBuff);

        return false;
    }
    
    initRippleCoeff();
    
    initMesh();
    
    return true ;
}

void RippleModel::setRippleStrength(bool repeat, float strength) 
{
    isRepeat = repeat ;
    rippleStrength = strength ;
}

void RippleModel::runSimulation() 
{
    
    if (! isNeedSimulation)
        return;

 	isNeedSimulation = false;//fist assume no need to simulate
	float resultModify = (isRepeat) ? 0.5f : (0.5f - 0.5f / rippleStrength);//if repeat then no Attenuation else with Attenuation
    CCLOG("rippleStrength:%f",(float)rippleStrength);
    static const float kOneDiv1048 = 1.0f / 1048.0f;

	int y0, y1, y2, y3;
	float a, b, c, d, result, s_offset, t_offset;
    for (unsigned y = 0; y < poolHeight; ++y)
    {
		y0 = y*poolWidth;
		y1 = (y)*(poolWidth+2);
		y2 = (y+2)*(poolWidth+2);
		y3 = (y+1)*(poolWidth+2);

        for (unsigned x = 0; x<poolWidth; ++x)
        {
			// * - denotes current pixel
			//
			//       a
			//     c * d
			//       b
			a = rippleSource[y1 + x+1];
            b = rippleSource[y2 + x+1];
            c = rippleSource[y3 + x];
            d = rippleSource[y3 + x+2];
            result = (a + b + c + d - 2*rippleDest[y3 + x+1])*resultModify;
			rippleDest[y3 + x+1] = result;
			if (result >0.05f || result < -0.05f) //if found a big value consider as need to simulate
			{
				isNeedSimulation = true;
			}

			// clamp
            s_offset = ((b - a) * kOneDiv1048);
			t_offset = ((c - d) * kOneDiv1048);
            rippleTexCoords[(y0 + x)*2] = (s_tcBuff[x] + s_offset);
            rippleTexCoords[(y0 + x)*2+1] = (t_tcBuff[y] + t_offset);

        }
    }

    float *pTmp = rippleDest;
    rippleDest = rippleSource;
    rippleSource = pTmp;   
}

void RippleModel::initiateRippleAtLocation(const cocos2d::CCPoint& location) 
{
    isNeedSimulation = true;
	
    unsigned int xIndex = (unsigned int)((location.x /textureWidth) * poolWidth) ;
    unsigned int yIndex = (unsigned int)((location.y /textureHeight) * poolHeight) ;

	unsigned int xBegin = xIndex - touchRadius;
	unsigned int xEnd = xIndex + touchRadius;

	unsigned int yBegin = yIndex - touchRadius;
	unsigned int yEnd = yIndex + touchRadius;

    for (unsigned int y=yBegin; y<=yIndex; ++y)
    {
        for (unsigned int x=xBegin; x<=xEnd; ++x)
        {        
            if (x>=0 && x<(int)poolWidth &&
                y>=0 && y<(int)poolHeight)
            {
                // +1 to both x/y values because the border is padded
                rippleSource[(poolWidth+2)*(y+1)+x+1] += rippleCoeff[(y-yBegin)*(touchRadius*2+1)+x-xBegin];   
            }
        }
    } 
}
