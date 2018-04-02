# Islandora IIIF Generator

## Introduction

This module produces [IIIF object](http://iiif.io/api/presentation/2.1) Islandora datastreams for the following Islandora Solution Packs:
 * [Islandora Book Solution Pack](https://github.com/Islandora/islandora_solution_pack_book)
 * [Islandora Paged Content](https://wiki.duraspace.org/display/ISLANDORA715/Islandora+Paged+Content)
 * [Islandora Large Image Solution Pack](https://wiki.duraspace.org/display/ISLANDORA715/Large+Image+Solution+Pack)
 * [Islandora Basic Image Solution Pack](https://wiki.duraspace.org/display/ISLANDORA715/Basic+Image+Solution+Pack)
 * [Islandora Basic Collection Solution Pack](https://wiki.duraspace.org/display/ISLANDORA715/Basic+Collection+Solution+Pack)
 * [Islandora Compound Solution Pack](https://wiki.duraspace.org/display/ISLANDORA715/Compound+Solution+Pack)

The generated IIIF object will be used by the [Mirador Bookreader](https://github.com/utlib/islandora_mirador_bookreader) module. 

This module development was supported by the The Andrew W. Mellon Foundation for the [French Renaissance Paleography website] (https://paleography.library.utoronto.ca/).

## Install
1. Clone this module into Drupal's module directory and enable.
2. Go to `admin/islandora/tools/sc_manifest` and enter the URLs for the following:
 * [IIIF Image API Base URI](http://iiif.io/api/image/2.0/)
 * [IIIF Presentation API Base URI](http://iiif.io/api/presentation/2.1/)

## Usage
There are two ways to generate IIIF objects: Via Drush or Via one of the above mentioned Solution Pack. Both generates a new datastream called **IIIF** in their target objects.

### Drush
On the command line, run the following drush command:
```
drush iiifgen PID_OF_OBJECT
```
where PID_OF_OBJECT is the PID of the object to generate IIIF.
To generate the IIIF for a Collection and all of its children, run the drush command with the `recursive` flag:
```
drush iiifgen PID_OF_OBJECT --recursive
```
For testing and debugging purposes, run the drush command with the `test` and `show` flag:
```
drush iiifgen PID_OF_OBJECT --test --show
```
where `test` will prevent the datastream being ingested and `show` will output the generated JSON in the console.

Example output when generating a Collection object will look like:
```
drush iiifgen paleography:historicalmaps --test --recursive

<<<<<<<-- Running in TEST mode. This will not ingest to Fedora. -->>>>>>>>

Processing IIIF Collection for Object: paleography:historicalmaps
Collection has 6 children

Processing Sub Manifest: paleography:423 for Collection: paleography:historicalmaps
Processing Sub Manifest: paleography:387 for Collection: paleography:historicalmaps
..............

<<<<<<<-- Running in Recursive mode. -->>>>>>>>

Processing Manifest paleography:423
Processing Canvas (Page) for Object: paleography:423

Processing Manifest paleography:423
Processing Canvas (Page) for Object: paleography:423

..............

Successfully generated the IIIF JSON for: paleography:historicalmaps                                                   [status]
Successfully generated the IIIF JSON for: paleography:423                                                              [status]
Successfully generated the IIIF JSON for: paleography:423                                                              [status]
..............
```

### UI
Each Comaptible Solution Pack object has a new **IIIF Generation** option under **Manage** through the admin UI for each object. The option exists as a sub-menu item.

Clicking **Re-generate IIIF** will add a new `IIIF` datastream for the current object. Subsequent usage will refresh the datastream.

## Current maintainers

* [University of Toronto Libraries:](http://its.library.utoronto.ca/)
	* [Kelli Babcock](http://its.library.utoronto.ca/staff/kelli-babcock)
	* [Bilal Khalid](https://github.com/bilalkhalid)
	* [Sunny Lee](https://github.com/sunnywd)
	* [Sean Xiao Zhao](https://github.com/sean-xiao-zhao7)
	* [Chulhee Yoon](https://github.com/cyoon84)

## Copyright and License

Copyright 2016 University of Toronto Libraries

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
