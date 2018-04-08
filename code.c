#include "argList.H"
#include "timeSelector.H"
#include "IOsampledSets.H"
#include "IOsampledSurfaces.H"

int main(int argc, char *argv[])
{
    timeSelector::addOptions();
    #include "addRegionOption.H"
    #include "addDictOption.H"
    #include "setRootCase.H"
    #include "createTime.H"
    instantList timeDirs = timeSelector::select0(runTime, args);
    #include "createNamedMesh.H"

    const word dictName("sampleDict");

    autoPtr<IOsampledSets> sSetsPtr;
    autoPtr<IOsampledSurfaces> sSurfsPtr;

    if (args.optionFound("dict"))
    {

        fileName dictPath = args["dict"];
        if (isDir(dictPath))
        {
            dictPath = dictPath / dictName;
        }

        sSetsPtr.reset
        (
            new IOsampledSets
            (
                sampledSets::typeName,
                mesh,
                dictPath,
                IOobject::MUST_READ_IF_MODIFIED,
                true
            )
        );


        sSetsPtr().checkOut();

        sSurfsPtr.reset
        (
            new IOsampledSurfaces
            (
                sampledSurfaces::typeName,
                mesh,
                dictPath,
                IOobject::MUST_READ_IF_MODIFIED,
                true
            )
        );
    }
    else
    {

        sSetsPtr.reset
        (
            new IOsampledSets
            (
                sampledSets::typeName,
                mesh,
                dictName,
                IOobject::MUST_READ_IF_MODIFIED,
                true
            )
        );

        sSetsPtr().checkOut();

        sSurfsPtr.reset
        (
            new IOsampledSurfaces
            (
                sampledSurfaces::typeName,
                mesh,
                dictName,
                IOobject::MUST_READ_IF_MODIFIED,
                true
            )
        );
    }

    IOsampledSets& sSets = sSetsPtr();
    IOsampledSurfaces& sSurfs = sSurfsPtr();

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info<< "Time = " << runTime.timeName() << endl;

        polyMesh::readUpdateState state = mesh.readUpdate();

        sSets.readUpdate(state);
        sSurfs.readUpdate(state);

        sSets.write();
        sSurfs.write();

        Info<< endl;
    }

    Info<< "End\n" << endl;

    return 0;
}
