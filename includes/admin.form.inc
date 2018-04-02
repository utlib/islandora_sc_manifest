<?php

/**
 * Creates the administration form 
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   A renderable array representing the islandora_mirador_bookreader administration form.
 */
function islandora_sc_manifest_admin_settings_form($form, &$form_state) {
  $get_default_value = function($name, $default) use($form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  //$solr_enabled = module_exists('islandora_solr');
  $form = array(
  'iiif_image_api_base_uri' => array(
    '#type' => 'textfield',
    '#title' => t('IIIF Image API Base URI'),
    '#default_value' => $get_default_value('iiif_image_api_base_uri', 'https://iiif.library.utoronto.ca/image/v2/'),
  ),
  'iiif_presentation_api_base_uri' => array(
    '#type' => 'textfield',
    '#title' => t('IIIF Presentation API Base URI'),
    '#default_value' => $get_default_value('iiif_presentation_api_base_uri', 'https://iiif.library.utoronto.ca/presentation/v2/'),
  )
  );
  return system_settings_form($form);
}
