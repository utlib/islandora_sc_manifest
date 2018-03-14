# Islandora IIIF Generator

## Introduction

This module produces [IIIF object](http://iiif.io/api/presentation/2.1) Islandora datastreams for the following Islandora Solution Packs:
 * [Islandora Book Solution Pack](https://github.com/Islandora/islandora_solution_pack_book)
 * [Islandora Large Image Solution Pack](https://wiki.duraspace.org/display/ISLANDORA715/Large+Image+Solution+Pack)
 * [Islandora Basic Image Solution Pack](https://wiki.duraspace.org/display/ISLANDORA715/Basic+Image+Solution+Pack)
 * [Islandora Basic Collection Solution Pack](https://wiki.duraspace.org/display/ISLANDORA715/Basic+Collection+Solution+Pack)
 * [Islandora Compound Solution Pack](https://wiki.duraspace.org/display/ISLANDORA715/Compound+Solution+Pack)

The generated IIIF object will be used by the [Mirador Bookreader](https://github.com/utlib/islandora_mirador_bookreader) module. 

This module development was supported by the The Andrew W. Mellon Foundation for the [French Renaissance Paleography website] (https://paleography.library.utoronto.ca/).

## Install
1. Clone this module into Drupal's module directory and enable.
3. Go to admin/islandora/tools/sc_manifest` and enter the URLs for the [IIIF Image API](http://iiif.io/api/image/2.0/) server and Fedora.

## Usage
There are two ways to generate manifests: Drush or individual Book Solution Pack object. Both generate a new datastream called **SC** in their target objects.
### Drush
On the command line, run the following drush command:
```
drush scgen PID_TO_REINDEX
```
where PID_TO_REINDEX is a single Book Solution Pack object.
Run the following to reindex the entire repository:
```
drush scgen FULL
```
### UI
Each Book Solution Pack object has a new **SC Manifest** option under **Manage** through the admin UI for each object. The option exists as a sub-menu item.

Clicking **Re-generate SC Manifest** will add a new SC datastream for the current object. Subsequent usage will refresh the datastream.

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
