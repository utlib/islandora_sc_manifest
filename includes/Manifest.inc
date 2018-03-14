<?php

class Manifest {
  protected $id = '';
  protected $manifest_label = '';
  protected $metadata = [];
  protected $metadata_uri = '';
  protected $canvases = array ();

  /**
   * Initiate the Manifest
   *
   * @param string $id '@id' url for the Manifest
   * @param string $manifest_label label for the Manifest
   * @param string $metadata_uri url for the 'MODS' datastream
   */
  function __construct($id, $manifest_label, $metadata_uri) {
    $this->id = $id;
    $this->manifest_label = $manifest_label;
    $this->metadata_uri = $metadata_uri;
  }

  /**
   * Set the Manifest metadata from $metadatas array
   *
   * @param array $metadatas
   */
  function setMetadata($metadatas) {
    foreach($metadatas as $key => $values) {
      $key_values = [];
      foreach($values as $value) $key_values [] = "$value";
      $this->metadata [] = array ('label' => $key, 'value' => $key_values);
    }
  }

  /**
   * Add the Canvas to the Sequence (Manifest conatins only 1 Sequence)
   *
   * @param string $canvas
   */
  function addCanvas($canvas = null) {
    if ($canvas != null) {
      $this->canvases [] = $canvas->toArray();
    }
  }

  /**
   * Build and return a json string based on what we have in the class for the IIIF Manifest
   */
  function getJson() {
    $scManifest = array (
      '@context' => 'http://iiif.io/api/presentation/2/context.json',
      '@id' => $this->id,
      '@type' => 'sc:Manifest',
      'label' => $this->manifest_label,
      'metadata' => $this->metadata,
      'attribution' => 'For rights and reproduction information please contact collections@library.utoronto.ca',
      'seeAlso' => array ('@id' => $this->metadata_uri, 'format' => 'application/rdf+xml'),
      'sequences' => array ( // an array but will always be a single object in our application
        array (
          '@type' => 'sc:Sequence',
          'label' => $this->manifest_label . ', in order',
          'canvases' => $this->canvases
        )
      )
    );
    return json_encode ( $scManifest );
  }
}
