// Copyright (c) 2008-1016, NICTA (National ICT Australia).
// Copyright (c) 2016, Commonwealth Scientific and Industrial Research
// Organisation (CSIRO) ABN 41 687 119 230.
//
// Licensed under the CSIRO Open Source Software License Agreement;
// you may not use this file except in compliance with the License.
// Please see the file LICENSE, included with this distribution.
//
#ifndef TRANSCMDTRIMRELATIVE_HH
#define TRANSCMDTRIMRELATIVE_HH

#ifndef GOSSCMD_HH
#include "GossCmd.hh"
#endif

class TransCmdFactoryTrimRelative : public GossCmdFactory
{
public:
    GossCmdPtr create(App& pApp, const boost::program_options::variables_map& pOpts);

    TransCmdFactoryTrimRelative();
};

#endif // TRANSCMDTRIMRELATIVE_HH
