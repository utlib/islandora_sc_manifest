<?php

/**
* Generate a IIIF Manifest of $object_pid and create/re-create the 'IIIF' datastream
*
* @param string $object_pid persistent identifier of the digital object.
* @param boolean $from_drush whether this function was invoked from the drush command line.
* @param boolean $debug whether to run the script with DEBUG mode. This will not create the 'IIIF' datastream.
*/
function __generate_iiif_manifest($object_pid, $from_drush = FALSE, $debug = FALSE) {

  require_once ("utlfedoraobject.inc");
  require_once ("Collection.inc");
  require_once ("Manifest.inc");
  require_once ("Canvas.inc");
  require_once ("Metadata.inc");

  global $base_url;
  $iiif_image_api_base_uri = variable_get('iiif_image_api_base_uri', 'https://iiif.library.utoronto.ca/image/v2/'); 
  $iiif_presentation_api_base_uri = variable_get('iiif_image_api_base_uri', 'https://iiif.library.utoronto.ca/presentation/v2/'); 

  $user = user_load(1);
  $fo = new UTLFedoraObject($user->name, $user->pass);
  $api_a = $fo->get_api_a(); // Get Fedora repository's Access API.
  $api_m = $fo->get_api_m(); // Get Fedora repository's Manage API.

  $object = $fo->get_object($object_pid);
  $iiif_manifestModels = array('islandora:bookCModel', 'islandora:sp_basic_image', 'islandora:sp_large_image_cmodel');
  $iiif_collectionModels = array('islandora:collectionCModel', 'islandora:compoundCModel');

  // Create a IIIF Collection if object's model is of type islandora:collectionCModel or islandora:compoundCModel
  if (in_array('islandora:collectionCModel', $object->models) || in_array('islandora:compoundCModel', $object->models))  {
    if ($from_drush) echo "Processing IIIF Collection for Object: $object_pid\n";

    // Create the Collection object
    $collection = new Collection(
      $iiif_presentation_api_base_uri.'collection/'.$object_pid, 
      $object->label, 
      "$base_url/islandora/object/$object_pid/datastream/MODS"
    );

    // Create the metadatas for the Collection
    $datastream_rdf_xml_string = $api_a->getDatastreamDissemination($object_pid, 'DC'); // 'DC' datastream as a RDF-XML string.
    $metadatas = get_metadatas($datastream_rdf_xml_string); // Array of (key=>values) metadatas
    $collection->setMetadata($metadatas);

    if (in_array('islandora:collectionCModel', $object->models)){
      $children = $fo->get_collection_children($object_pid); // Get all the children objects of this Collection
    } else { // 'islandora:collectionCModel'
      $children = $fo->get_compound_children($object_pid); // Get all the children objects of this Compound Object
    }

    if ($from_drush) echo "Collection has ".sizeof($children)." children\n\n";
    foreach($children as $child) {    
      $child_pid = $child['pid']['value'];
      $child_label = $child['label']['value'];
      $child_model_type = $child['models']['value'];
      if (in_array($child_model_type, $iiif_collectionModels)){
        if ($from_drush) echo "Processing Sub Collection: $child_pid for Collection: $object_pid\n";
        // Create the Sub Collection object for the Collection. (Partial: Not the entire Collection)
        $sub_collection = array(
          '@id' => $iiif_presentation_api_base_uri.'collection/'.$child_pid, 
          '@type' => 'sc:Collection',
          'label' => $child_label
        );
        $collection->addCollection($sub_collection);
      } else if (in_array($child_model_type, $iiif_manifestModels)) {
        if ($from_drush) echo "Processing Sub Manifest: $child_pid for Collection: $object_pid\n";
        // Create the Sub Manifest object for the Collection. (Partial: Not the entire Manifest)
        $manifest = array(
          '@id' => $iiif_presentation_api_base_uri.$child_pid.'/manifest',
          '@type' => 'sc:manifest',
          'label' => $child_label
        );
        $collection->addManifest($manifest);
      }
    }
    $json = $collection->getJson(); // Generate the IIIF Collection JSON
  }

  // Create a IIIF Collection if object's model is of type islandora:compoundCModel
  else if (in_array('islandora:compoundCModel', $object->models)) {
    // TODO: Need example of a compoundCModel in Paleography to test and develop --Jana
    if ($from_drush) echo "Processing IIIF Collection for Object: $object_pid\n";
    if ($from_drush) echo "TODO: Not Yet Implemented\n";
  }

  else { // Create the IIIF Manifest
    if ($from_drush) echo "Processing Manifest $object_pid\n";

    // Create the Manifest object
    $manifest = new Manifest(
      $iiif_presentation_api_base_uri.$object_pid.'/manifest.json', 
      $object->label, 
      "$base_url/islandora/object/$object_pid/datastream/MODS"
    );

    // Create the metadatas for the Manifest
    $datastream_rdf_xml_string = $api_a->getDatastreamDissemination($object_pid, 'DC'); // 'DC' datastream as a RDF-XML string.
    $metadatas = get_metadatas($datastream_rdf_xml_string); // Array of (key=>values) metadatas
    $manifest->setMetadata($metadatas);

    // Get the pages ordered by sequence number if the object model is of type islandora:bookCModel
    if (in_array('islandora:bookCModel', $object->models)) {
      module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
      $pages = islandora_paged_content_get_pages($object);
      foreach($pages as $page) {
        $page_pid = $page['pid'];
        if ($from_drush) echo "Processing Canvas(Page): $page_pid of Object: $object_pid\n";
        $page_object = $fo->get_object($page_pid);
          
        // Create the Canvas object for the Manifest
        $canvas_uri = $iiif_presentation_api_base_uri."$object_pid/canvas/$page_pid";
        $thumbnail_uri = $iiif_image_api_base_uri."$page_pid/full/80,/0/default.jpg";
        $canvas = new Canvas($canvas_uri, $page_object->label, $thumbnail_uri, "$base_url/islandora/object/$object_pid/datastream/MODS");

        // Create the Resource(Annotation) object for the Canvas
        $resource_uri = $iiif_image_api_base_uri."$page_pid/full/full/0/default.jpg";
        $service_uri = $iiif_image_api_base_uri."$page_pid";
        $mimetype = "image/jpg";
        $page_object_rels = $page_object['JP2']->relationships;
        $width = $page_object['JP2']->relationships->get('http://islandora.ca/ontology/relsext#', 'width')[0]['object']['value'];
        $height = $page_object['JP2']->relationships->get('http://islandora.ca/ontology/relsext#', 'height')[0]['object']['value'];
        $canvas->setResource($resource_uri, $service_uri, $mimetype, $width, $height);

        // Create the metadatas for the Canvas
        $datastream_rdf_xml_string = $api_a->getDatastreamDissemination($page_pid, 'DC'); // 'DC' datastream as a RDF-XML string.
        $metadatas = get_metadatas($datastream_rdf_xml_string); // Array of (key=>values) metadatas
        $canvas->setMetadata($metadatas);

        // Add the Canvas to the Sequence (Manifest conatins only 1 Sequence)
        $manifest->addCanvas($canvas);
      }
    }

    // Get the single object if its model is of type islandora:sp_basic_image or islandora:sp_large_image_cmodel
    if (in_array('islandora:sp_basic_image', $object->models) || in_array('islandora:sp_large_image_cmodel', $object->models)) {
      if ($from_drush) echo "Processing Canvas (Page) for Object: $object_pid\n";

      $parents = $object->getParents();

      // Create the Canvas object for the Manifest
      $canvas_uri = $iiif_presentation_api_base_uri."$parents[0]/canvas/$object_pid";
      $thumbnail_uri = $iiif_image_api_base_uri."$object_pid/full/80,/0/default.jpg";
      $canvas = new Canvas($canvas_uri, $object->label, $thumbnail_uri, "$base_url/islandora/object/$object_pid/datastream/MODS");

      // Create the Resource(Annotation) object for the Canvas
      $resource_uri = $iiif_image_api_base_uri."$object_pid/full/full/0/default.jpg";
      $service_uri = $iiif_image_api_base_uri."$object_pid";
      $mimetype = "image/jpeg";
      $width = $object->relationships->get('http://islandora.ca/ontology/relsext#', 'width')[0]['object']['value'];
      $height = $object->relationships->get('http://islandora.ca/ontology/relsext#', 'height')[0]['object']['value'];
      $canvas->setResource($resource_uri, $service_uri, $mimetype, $width, $height);

      // Create the metadatas for the Canvas
      $datastream_rdf_xml_string = $api_a->getDatastreamDissemination($object_pid, 'DC'); // 'DC' datastream as a RDF-XML string.
      $metadatas = get_metadatas($datastream_rdf_xml_string); // Array of (key=>values) metadatas
      $canvas->setMetadata($metadatas);

      // Add the Canvas to the Sequence (Manifest conatins only 1 Sequence)
      $manifest->addCanvas($canvas);
    }
    $json = $manifest->getJson(); // Generate the IIIF Manifest JSON
  }


  if ($debug) {
    echo json_encode(json_decode($json), JSON_PRETTY_PRINT) . "\n\n";
  }else {
    // For some reason, the previous IIIF datastream needs to be purged first. Otherwise add_datasteam() create multiple IIIF datastreams
    $object->purgeDatastream('IIIF');
    // Create new datastream only if it doesn't already exist (or if the force flag is set)
    $fo->add_datastream($object, "IIIF", "IIIF Representation", "application/json", $json);
  }

  drupal_set_message(t('Successfully generated the IIIF JSON.'));
}
