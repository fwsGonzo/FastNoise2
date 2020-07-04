#pragma once

#include "Generator.h"

namespace FastNoise
{
    class Cellular : public virtual Generator
    {
    public:
        enum class DistanceFunction
        {
            Euclidean,
            EuclideanSquared,
            Manhattan,
            Natural,
        };

        void SetJitterModifier( const std::shared_ptr<Generator>& gen ) { this->SetSourceMemberVariable( mJitterModifier, gen ); }
        void SetJitterModifier( float value ) { mJitterModifier = value; }
        void SetDistanceFunction( DistanceFunction value ) { mDistanceFunction = value; }

    protected:
        HybridSource mJitterModifier = 1.0f;
        DistanceFunction mDistanceFunction = DistanceFunction::EuclideanSquared;

        const float kJitter2D = 0.5f;
        const float kJitter3D = 0.45f;

        FASTNOISE_METADATA_ABSTRACT( Generator )
        
            Metadata( const char* className ) : Generator::Metadata( className )
            {
                this->AddHybridSource( "Jitter Modifier", 1.0f, &Cellular::SetJitterModifier, &Cellular::SetJitterModifier );
                this->AddVariableEnum( "Distance Function", DistanceFunction::EuclideanSquared, &Cellular::SetDistanceFunction, "Euclidean", "Euclidean Squared", "Manhattan", "Natural" );
            }
        };
    };

    class CellularValue : public virtual Cellular
    {
        FASTNOISE_METADATA( Cellular )
            using Cellular::Metadata::Metadata;
        };
    };

    class CellularDistance : public virtual Cellular
    {
    public:
        enum class ReturnType
        {
            Index0,
            Index0Add1,
            Index0Sub1,
            Index0Mul1,
            Index0Div1,
        };

        void SetDistanceIndex0( int value ) { mDistanceIndex0 = value; }
        void SetDistanceIndex1( int value ) { mDistanceIndex1 = value; }
        void SetReturnType( ReturnType value ) { mReturnType = value; }

    protected:
        static const int kMaxDistanceCount = 4;

        ReturnType mReturnType = ReturnType::Index0;
        int mDistanceIndex0 = 0;
        int mDistanceIndex1 = 1;

        FASTNOISE_METADATA( Cellular )

            Metadata( const char* className ) : Cellular::Metadata( className )
            {
                this->AddVariable( "Distance Index 0", 0, &CellularDistance::SetDistanceIndex0, 0, kMaxDistanceCount - 1 );
                this->AddVariable( "Distance Index 1", 1, &CellularDistance::SetDistanceIndex1, 0, kMaxDistanceCount - 1 );
                this->AddVariableEnum( "Return Type", ReturnType::Index0, &CellularDistance::SetReturnType, "Index0", "Index0Add1", "Index0Sub1", "Index0Mul1", "Index0Div1" );
            }
        };
    };

    class CellularLookup : public virtual Cellular
    {
    public:
        void SetLookup( const std::shared_ptr<Generator>& gen ) { this->SetSourceMemberVariable( mLookup, gen ); }
        void SetLookupFrequency( float freq ) { mLookupFreq = freq; }

    protected:
        GeneratorSource mLookup;
        float mLookupFreq = 0.1f;

        FASTNOISE_METADATA( Cellular )
        
            Metadata( const char* className ) : Cellular::Metadata( className )
            {
                this->AddGeneratorSource( "Lookup", &CellularLookup::SetLookup );
                this->AddVariable( "Lookup Frequency", 0.1f, &CellularLookup::SetLookupFrequency );
            }
        };
    };
}
