<?php

class Canvas {
  protected $id;
  protected $canvas_label;
  protected $thumbnail_uri;
  protected $metadata = [];
  protected $metadata_uri = '';
  protected $resource_uri;
  protected $service_uri;
  protected $image_format;
  protected $image_height;
  protected $image_width;

  /**
   * Initiate the Canvas
   *
   * @param string $id '@id' url for the Canvas
   * @param string $canvas_label label for the Canvas
   * @param string $thumbnail_uri url for the Resource thumbnail
   * @param string $metadata_uri url for the 'MODS' datastream
   */
  function __construct($id, $canvas_label, $thumbnail_uri, $metadata_uri) {
    $this->id = $id;
    $this->canvas_label = $canvas_label;
    $this->thumbnail_uri = $thumbnail_uri;
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
   * Create the Resource(Annotation) for the Canvas
   *
   * @param string $resource_uri url of the image
   * @param string $service_uri url of the IIIF service
   * @param string $format format of the resource; defaults to 'image/jpeg'
   * @param int $width width of the image resource in pixels
   * @param int $height height of the image resource in pixels
   */
  function setResource($resource_uri, $service_uri, $format = 'image/jpeg', $width, $height) {
    $this->resource_uri = $resource_uri;
    $this->service_uri = $service_uri;
    $this->image_format = $format;
    $this->image_width = intval($width);
    $this->image_height = intval($height);
  }

  /**
   * Return an associative array based on what we have in the class for the IIIF Canvas
   */
  function toArray() {
    $returnArray = array (
      '@type' => 'sc:Canvas',
      '@id' => $this->id,
      'label' => $this->canvas_label,
      'height' => $this->image_height,
      'width' => $this->image_width,
      'metadata' => $this->metadata,
      'thumbnail' => array (
        '@id' => $this->thumbnail_uri,
        'service' => array (
          '@context' => 'http://iiif.io/api/image/2/context.json',
          '@id' => $this->service_uri,
          'profile' => 'http://iiif.io/api/image/2/level2.json'
        )
      ),
      'images' => array ( // always contain only one
        array (
          '@type' => 'oa:Annotation',
          'motivation' => 'sc:Painting',
          'on' => $this->id,
          'resource' => array (
            '@id' => $this->resource_uri,
            '@type' => 'dctypes:Image',
            'format' => $this->image_format,
            'height' => $this->image_height,
            'width' => $this->image_width,
            'service' => array (
              '@context' => 'http://iiif.io/api/image/2/context.json',
              '@id' => $this->service_uri,
              'profile' => 'http://iiif.io/api/image/2/level2.json'
             )
          )
        )
      ),
      'seeAlso' => array ('@id' => $this->metadata_uri, 'format' => 'application/rdf+xml')
    );
    return $returnArray;
  }
}
