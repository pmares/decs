%%% DECS GPenSIM - Generic Distributed Exact Cover Solver (DECS) Petri net model
%%% Copyright (C) 2007  Jan Magne Tjensvold
%%% 
%%% This program is free software; you can redistribute it and/or modify it
%%% under the terms of the GNU General Public License version 2 as published
%%% by the Free Software Foundation.
%%% 
%%% This program is distributed in the hope that it will be useful, but
%%% WITHOUT ANY WARRANTY; without even the implied warranty of
%%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
%%% General Public License for more details.
%%% 
%%% You should have received a copy of the GNU General Public License along
%%% with this program; if not, write to the Free Software Foundation, Inc.,
%%% 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
%%% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% File: server_def.m: Definition of the Server Module
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [PN_name, set_of_places, set_of_trans, set_of_arcs] ...
    = server_def(global_info)

PN_name = 'Server Module';
set_of_places = {'pREQ', 'pDIV', 'pDIST', 'pCOL', 'pRTR', 'pRES'};
set_of_trans = {'tTR', 'tDIV', 'tMRG', 'tRTR'};
set_of_arcs = { ...
    'pREQ','tTR',1, ...
    'tTR','pDIV',1, ...
    'pDIV','tDIV',1, ...
    'tDIV','pDIST',global_info.pieces, ...
    'pCOL','tMRG',global_info.pieces, ...
    'tMRG','pRTR',1, ...
    'pRTR','tRTR',1, ...
    'tRTR','pRES',1, ...
    };
