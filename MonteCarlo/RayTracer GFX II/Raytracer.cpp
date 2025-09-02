#include <math.h>
/***************************************************************************
*                                                                          *
* This is the source file for a ray tracer. It defines most of the		   *
* fundamental functions using in ray tracing.  In particular, "Trace" and  *
* "Shade".  It also defines the MakeImage function, which casts all the    *
* rays from the eye, and several object intersection methods.  Some of the *
* functions are left blank, or with "place holders" that don't do very     *
* much.  You need to fill these in.                                        *
*                                                                          *
*                                                                          *
***************************************************************************/

static const int max_recursions = 1;		// Number of recursions to compute indirect illumination

static const int rays_pixel = 10;

#include "Raytracer.h"

// Programmed by: ______________________ nombres

Color Raytracer::Shade(const HitInfo& hit, const Scene& scene, int max_recursion) {
	

		// TODO: WRITE YOUR CODE HERE


		return hit.material.diffuse;
	
}



// Draw image on the screen
void Raytracer::draw( void )
{
	glDrawPixels( resolutionX, resolutionY, GL_RGB, GL_UNSIGNED_BYTE, &(*I)( 0 , 0 ) );
}

// Cast_line casts all the initial rays starting from the eye for a single
//  raster line. Copies pixels to image object.
void Raytracer::cast_line( World world )
{
    Ray ray;
	Color color;	// Color computed when using multiple rays per pixel

	ray.origin = world.getCamera().eye; // All initial rays originate from the eye.
	ray.no_emitters = false;

    Vec3 G  = Unit( world.getCamera().lookat - world.getCamera().eye );	// Gaze direction.
    Vec3 U  = Unit( world.getCamera().up / G );							// Up vector.
    Vec3 R  = Unit( G ^ U );											// Right vector.
    Vec3 O  = ( world.getCamera().vpdist * G ) - R + U;					// "Origin" for the raster.
    Vec3 dU = U * ( 2.0 / ( resolutionY - 1 ) );						// Up increments.
	Vec3 dR = R * ( 2.0 / ( resolutionX - 1 ) );						// Right increments.

    if( currentLine % 10 == 0 ) cout << "line " << currentLine << endl;
    for( int i = 0; i < resolutionX; i++ )
    {
		if( rays_pixel == 1 )
		{
			// One ray per pixel
			ray.direction = Unit( O + i * dR - currentLine * dU  );
			color = Trace( ray, world.getScene(), max_recursions );
		}
		else
		{
			// Multisampling
			for( int n = 0 ; n < rays_pixel ; n++ )
			{
				ray.direction = Unit( O + ( i + rand( 0.0 , 1.0 ) - 0.5 ) * dR - ( currentLine + rand( 0.0 , 1.0 ) - 0.5 ) * dU  );
				color += Trace( ray, world.getScene(), max_recursions);
			}
		}
		(*I)( resolutionY-currentLine-1, i ) = ToneMap( color / rays_pixel );

		color.blue = 0;
		color.green = 0;
		color.red = 0;
    }

	if (++currentLine == resolutionY)
	{
		// Image computation done, save it to file
		cout << "done." << endl;
	    I->Write( "Resultat.ppm" );
		isDone = true;
	}
}


// This is a trivial tone mapper; it merely maps values that are
// in [0,1] and maps them to integers between 0 and 255.  If the
// real value is above 1, it merely truncates.  A true tone mapper
// would attempt to handle very large values nicely, without
// truncation; that is, it would try to compensate for the fact that
// displays have a very limited dynamic range.
Pixel Raytracer::ToneMap( const Color &color )
{
	int red   = (int)floor( 256 * color.red   );
    int green = (int)floor( 256 * color.green );
    int blue  = (int)floor( 256 * color.blue  );
    channel r = (channel)( red   >= 255 ? 255 : red   ); 
    channel g = (channel)( green >= 255 ? 255 : green ); 
    channel b = (channel)( blue  >= 255 ? 255 : blue  );
    return Pixel( r, g, b );
}

// Trace is the most fundamental of all the ray tracing functions.  It
// answers the query "What color do I see looking along the given ray
// in the current scene?"  This is an inherently recursive process, as
// trace may again be called as a result of the ray hitting a reflecting
// object.  To prevent the possibility of infinite recursion, a maximum
// depth is placed on the resulting ray tree.
Color Raytracer::Trace( const Ray &ray, const Scene &scene, int max_recursions  )
{
    Color   color;                    // The color to return.
    HitInfo hitinfo;                  // Holds info to pass to shader.

	// Intitallizes hit distance to infinity to allow finding intersections in all ray length
	hitinfo.geom.distance = Infinity;

	if (Cast( ray, scene, hitinfo ) > 0.0f && max_recursions > -1 )
	{
        // The ray hits an object, so shade the point that the ray hit.
        // Cast has put all necessary information for Shade in "hitinfo".
		
		// If the ray has no_emitters activated and the first hit is an emitter
		//  this ray shouldn't contribute to the color of the current pixel
		if( hitinfo.material.isEmitter() && ray.no_emitters == true ) color = Color ();

		// The ray hits an object, so shade the point that the ray hit.
        // Cast has put all necessary information for Shade in "hitinfo".
		else color = Shade( hitinfo, scene, max_recursions - 1  );
    }
    else
    {
        // Either the ray has failed to hit anything, or
        // the recursion has bottomed out.
        color = scene.bgcolor;
    }
    
    return color;
}

// Cast finds the first point of intersection (if there is one)
// between a ray and a list of geometric objects.  If no intersection
// exists, the function returns false.  Information about the
// closest object hit is returned in "hitinfo". 
int Raytracer::Cast( const Ray &ray, const Scene &scene, HitInfo &hitinfo, Object *ignore )
{
	int hit = false;

    // Each intersector is ONLY allowed to write into the "HitGeom"
    // structure if it has determined that the ray hits the object
    // at a CLOSER distance than currently recorded in HitGeom.distance.
    // When a closer hit is found, the material fields of the "HitInfo"
    // structure are updated to hold the material of the object that 
    // was just hit.

    for( Object *object = scene.first; object != NULL; object = object->next )
    {
        if( object != ignore && object->Intersect( ray, hitinfo.geom ) )
            {
            hitinfo.material = object->material;  // Material of closest surface.
            hit = true;                           // We have hit an object.
            }
    }
    return hit;
}


// Returns a sample into the projected hemisphere. It is a type of importance sampling,
// using cosine projection around the normal. The projection up to the sphere is done in
// tangent space (z = up) so we must reflect the sample around the vector halfway between
// z axis and normal in order to convert to correct frame of reference
Sample Raytracer::sampleProjectedHemisphere( const Vec3 &N )
{
	Sample sample;
	double s_rand, t_rand;
	Vec3 aux, aux_mig;
	//start in tangent space
	Vec3 v(0.0,0.0,1.0);
	
	//weight of sample - according to PDF equations, this is Pi for a hemisphere
	sample.weight = Pi;
	
	//random values for s and t in parameter space
	s_rand = rand(0.0,1.0);
	t_rand = rand(0.0,1.0);
	
	//Projection upwards to hemisphere
	aux.x = sqrt(t_rand)*cos(2.0*Pi*s_rand);
	aux.y = sqrt(t_rand)*sin(2.0*Pi*s_rand);
	aux.z = 1.0 - pow(aux.x , 2.0) - pow(aux.y , 2.0);
	aux.z = (aux.z > 0.0) ? sqrt(aux.z) : 0.0;

	//Now convert to correct space (i.e. around normal)
	aux_mig = Unit(v + N);
	sample.point = Reflection(-aux,aux_mig);
	
	return sample;
}

// Returns a sample into the specular lobe. The basic idea is to sample in a lobe
// formed by raising a sin and cos sphere to the power represented by phong_exp.
// We calculate the lobe in tangent space first (z = up) and then transform it to
// so that it is around the reflection vector R
Sample Raytracer::sampleSpecularLobe( const Vec3 &R, float phong_exp  )
{
	//Various local variables required
	Sample final;
	double s;
	double t;
	double spri;
	double e;
	Vec3 v(0.0,0.0,1.0);
	Vec3 aux_mig;
	
	//Two random samples in st parameter space
	s = rand(0.0,1.0);
	t = rand(0.0,1.0);
	
	
	//Calculate normal
	e= 2.0/(phong_exp+1.0);
	spri = sqrt(1.0-pow(s,e));

	final.point.x = spri* cos(2.0*Pi*t); 
	final.point.y = spri* sin(2.0*Pi*t); 
	final.point.z = sqrt(1.0 - pow(final.point.x , 2.0) - pow(final.point.y , 2.0));

	aux_mig = (v + R)/Length(v +R);

	//Calculate final vector
	final.point = Reflection(-final.point, aux_mig);
	//Don't forget the weight
	final.weight = (2.0*Pi)/(phong_exp+2.0);

	//Retornem la mostra.
	return final;
}




