<?php

/**
* Generate a IIIF Manifest of $object_pid and create/re-create the 'SC' datastream
*
* @param string $object_pid persistent identifier of the digital object.
* @param boolean $from_drush whether this function was invoked from the drush command line.
* @param boolean $debug whether to run the script with DEBUG mode. This will not create the 'SC' datastream.
*/
function __generate_iiif_manifest($object_pid, $from_drush = FALSE, $debug = FALSE) {

  require_once ("utlfedoraobject.inc");
  require_once ("Manifest.inc");
  require_once ("Canvas.inc");
  require_once ("Metadata.inc");

  global $base_url;
  $islandora_sc_manifest_image_uri = variable_get('islandora_sc_manifest_image_uri', 'https://iiif.library.utoronto.ca/image/v2/'); 
  $islandora_sc_manifest_presentation_uri = variable_get('islandora_sc_manifest_image_uri', 'https://iiif.library.utoronto.ca/presentation/v2/'); 

  $user = user_load(1);
  $fo = new UTLFedoraObject($user->name, $user->pass);
  $api_a = $fo->get_api_a(); // Get Fedora repository's Access API.
  $api_m = $fo->get_api_m(); // Get Fedora repository's Manage API.

  $object = $fo->get_object($object_pid);
  if ($from_drush) echo "Processing Manifest $object_pid\n";

  // Create the Manifest object
  $manifest = new Manifest(
    $islandora_sc_manifest_presentation_uri.$object_pid.'/manifest.json', 
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
      $canvas_uri = $islandora_sc_manifest_presentation_uri."$object_pid/canvas/$page_pid";
      $thumbnail_uri = $islandora_sc_manifest_image_uri."$page_pid/full/80,/0/default.jpg";
      $canvas = new Canvas($canvas_uri, $page_object->label, $thumbnail_uri, "$base_url/islandora/object/$object_pid/datastream/MODS");

      // Create the Resource(Annotation) object for the Canvas
      $resource_uri = $islandora_sc_manifest_image_uri."$page_pid/full/full/0/default.jpg";
      $service_uri = $islandora_sc_manifest_image_uri."$page_pid";
      $mimetype = "image/jpg";
      $page_object_rels = $page_object['JP2']->relationships;
      $width_rel = $page_object_rels->get('http://islandora.ca/ontology/relsext#', 'width');
      $height_rel = $page_object_rels->get('http://islandora.ca/ontology/relsext#', 'height');
      $width = $width_rel[0]['object']['value'];
      $height = $height_rel[0]['object']['value'];
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
    if ($from_drush) echo "Processing Single Canvas(Page) for Object: $object_pid\n";

    // Create the Canvas object for the Manifest
    $canvas_uri = $islandora_sc_manifest_presentation_uri."$object_pid/canvas/$object_pid";
    $thumbnail_uri = $islandora_sc_manifest_image_uri."$object_pid/full/80,/0/default.jpg";
    $canvas = new Canvas($canvas_uri, $object->label, $thumbnail_uri, "$base_url/islandora/object/$object_pid/datastream/MODS");

    // Create the Resource(Annotation) object for the Canvas
    $resource_uri = $islandora_sc_manifest_image_uri."$object_pid/full/full/0/default.jpg";
    $service_uri = $islandora_sc_manifest_image_uri."$object_pid";
    $mimetype = "image/jpeg";
    $width = 5574; // TODO: Get actual width from object
    $height = 3636; // TODO: Get actual height from object
    $canvas->setResource($resource_uri, $service_uri, $mimetype, $width, $height);

    // Create the metadatas for the Canvas
    $datastream_rdf_xml_string = $api_a->getDatastreamDissemination($object_pid, 'DC'); // 'DC' datastream as a RDF-XML string.
    $metadatas = get_metadatas($datastream_rdf_xml_string); // Array of (key=>values) metadatas
    $canvas->setMetadata($metadatas);

    // Add the Canvas to the Sequence (Manifest conatins only 1 Sequence)
    $manifest->addCanvas($canvas);
  }

  // Create a IIIF Collection if object's model is of type islandora:compoundCModel
  if (in_array('islandora:compoundCModel', $object->models)) {
    // TODO: Need example of a compoundCModel in Paleography to test and develop --Jana
    if ($from_drush) echo "Processing IIIF Collection for Object: $object_pid\n";
    if ($from_drush) echo "TODO: Not Yet Implemented\n";
  }

  // Generate the IIIF Manifest JSON
  $json = $manifest->getJson();

  if ($debug) {
    echo "\nSuccessfully created the IIIF Manifest.\n";
    echo json_encode(json_decode($json), JSON_PRETTY_PRINT) . "\n";
  }else {
    if ($from_drush) echo "\nSuccessfully created the IIIF Manifest.\n";
    // For some reason, the previous SC datastream needs to be purged first. Otherwise add_datasteam() create multiple SC datastreams
    $object->purgeDatastream('SC');
    // Create new datastream only if it doesn't already exist (or if the force flag is set)
    $fo->add_datastream($object, "SC", "SharedCanvas Manifest", "application/json", $json);
  }

  drupal_set_message(t('SC Manifest created'));
}
